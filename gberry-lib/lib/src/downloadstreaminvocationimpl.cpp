/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "downloadstreaminvocationimpl.h"

#include <QUrl>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDataStream>

#include "httpinvocationdefinition.h"
#include "networkerrorreasons.h"

#define LOG_AREA "DownloadStreamInvocationImpl"
#include "log/log.h"

namespace {
    const int READ_BUFFER_SIZE = 10000;
}

namespace GBerry {

class DownloadStreamInvocationImplPrivate
{
public:
    DownloadStreamInvocationImplPrivate(DownloadStreamInvocation* q_ ,InvocationFactoryImpl* factory) :
        q(q_),
        invocationFactory(factory) {
        readBuffer = new char[READ_BUFFER_SIZE]; // TODO: somehow tunable parameter
    }
    ~DownloadStreamInvocationImplPrivate() {
        delete readBuffer;
    }

    DownloadStreamInvocation* q;
    InvocationFactoryImpl* invocationFactory;
    Invocation::InvocationStatus invocationStatus{Invocation::NOT_STARTED};
    HTTPInvocationDefinition::Status httpStatus{HTTPInvocationDefinition::UNDEFINED};
    QString responseErrorData;
    // TODO: encoding,  content-type
    QNetworkReply* qreply{nullptr};
    qint64 totalReadBytes{0};

    QUrl url;
    QMap<QString, QString> params;
    HTTPInvocationDefinition def;
    Result result;

    QByteArray postData;
    QString outputFilePath;
    QFile outputFile;
    char* readBuffer;
    qint64 bytesReceived;
    qint64 bytesTotal;


    void doOperation() {
        doOperation(invocationFactory->buildUrl(def.invocationPath(), params));
    }

    void doOperation(QUrl url) {
        result << Result::Meta("url", url.toString());

        switch (def.httpOperation()) {
            case HTTPInvocationDefinition::GET:
                result << Result::Meta("http_operation", "GET");
                get(url);
                break;

            case HTTPInvocationDefinition::POST:
                result << Result::Meta("http_operation", "POST");
                post(url);
                break;

            case HTTPInvocationDefinition::NOT_DEFINED:
                result << InvocationErrors::INVOCATION_INVALID
                       << Result::reasonFromDesc("Http operation not defined");
                emit q->finishedError(q);
                break;

            default:
                result << InvocationErrors::INVOCATION_INVALID
                       << Result::reasonFromDesc("Unknown http operation");
                emit q->finishedError(q);
                break;
        }
    }

    void get(QUrl url) {
        // store for later use (e.g. redirects)
        this->url = url;

        // TODO: guard against having multiple queries ongoing (buffering them?), only one per this instance

        DEBUG("Executing GET operation");
        // TODO: is there signal for error situation
        qreply = invocationFactory->getQNetworkAccessManager()->get(QNetworkRequest(url));

        QObject::connect(qreply, &QNetworkReply::finished,
                [this] () {
            this->onQNetworkReplyFinished();
        });

        QObject::connect(qreply, &QNetworkReply::readyRead,
                [this] () {
            this->onQNetworkReplyReadyRead();
        });

        /* Download progress doesn't behave as expected -> call by ourselves when reading bytes
        QObject::connect(qreply, &QNetworkReply::downloadProgress,
                          [this] (qint64 bytesReceived, qint64 bytesTotal) {
            this->onQNetworkReplyDownloadProgress(bytesReceived, bytesTotal);
        });
        */

        invocationStatus = Invocation::ONGOING;
    }

    void post(QUrl url) {
        // store for redirects
        this->url = url;

        DEBUG("Executing POST operation");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        qreply = invocationFactory->getQNetworkAccessManager()->post(req, postData);

        QObject::connect(qreply, &QNetworkReply::finished,
                [this] () { this->onQNetworkReplyFinished(); });

        QObject::connect(qreply, &QNetworkReply::readyRead,
                [this] () {
            this->onQNetworkReplyReadyRead();
        });

        // TODO: this doesn't work reliable, not sure what is wrong, not correct headers
        //       FOR NOW: call from reading
        /*
        QObject::connect(qreply, &QNetworkReply::downloadProgress,
                          [this] (qint64 bytesReceived, qint64 bytesTotal) {
            this->onQNetworkReplyDownloadProgress(bytesReceived, bytesTotal);
        });
        */

        invocationStatus = Invocation::ONGOING;
    }

    void onQNetworkReplyReadyRead() {
        DEBUG("onQNetworkReplyReadyRead()");
        bool firstRead = invocationStatus != Invocation::RESPONSE_RECEIVED;
        invocationStatus = Invocation::RESPONSE_RECEIVED;

        QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if (statusCode.isValid())
        {
            httpStatus = HTTPInvocationDefinition::resolveHttpStatus(statusCode.toInt());
        } else {
            // TODO: what to do, when no code?
            invocationStatus = Invocation::ERROR;
            result << InvocationErrors::INVOCATION_FAILED
                   << Result::reasonFromDesc("Http status code not valid");
            return;
        }

        if (httpStatus != HTTPInvocationDefinition::OK_200) {
            invocationStatus = Invocation::ERROR;
            result << InvocationErrors::INVOCATION_FAILED
                   << Result::reasonFromDesc("Http error response code (not 200)")
                   << Result::Meta("http_code", statusCode.toString());

            // nothing to save to file
            DEBUG("Error no data to save to file");
            return;
        }

        // is target open?
        if (!outputFile.isOpen()) {
            if (!outputFile.open(QIODevice::WriteOnly)) {
                q->abort();
                result << InvocationErrors::INVOCATION_INVALID
                       << Result::reasonFromDesc("Open output file failed")
                       << Result::Meta("file_path", outputFilePath);

                invocationStatus = Invocation::INVALID_INVOCATION;
                emit q->finishedError(q);
                return;
            }
        }

        if (firstRead)
            emit q->downloadStarted(q);

        QVariant contentLength = qreply->header(QNetworkRequest::ContentLengthHeader);
        qint64 bytesTotal = contentLength.isValid() ? contentLength.toInt() : -1;

        int readBytes = 0;
        while ( (readBytes = qreply->read(readBuffer, READ_BUFFER_SIZE) ) > 0) {
            totalReadBytes += readBytes;
            outputFile.write(readBuffer, readBytes);
            onQNetworkReplyDownloadProgress(totalReadBytes, bytesTotal);
        }

    }

    void onQNetworkReplyDownloadProgress(qint64 bytesReceived_, qint64 bytesTotal_) {
        //DEBUG("### PROGRESS");
        bytesReceived = bytesReceived_;
        bytesTotal = bytesTotal_;
        emit q->downloadProgress(q);
    }

    void onQNetworkReplyFinished() {
        // either error or all data has been read
        DEBUG("httpFinished()");

        QVariant redirectionTarget = qreply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (!redirectionTarget.isNull())
        {
            QUrl newUrl = url.resolved(redirectionTarget.toUrl());
            DEBUG("Handled redirect to: " << newUrl.toString());
            qreply->deleteLater();
            qreply = nullptr;
            doOperation(newUrl);
            return;
        }
        else if (qreply->error())
        {
            WARN("HTTP ERROR: " << qreply->errorString());
            result << InvocationErrors::INVOCATION_FAILED;

            QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
            if (statusCode.isValid()) {
                httpStatus = HTTPInvocationDefinition::resolveHttpStatus(statusCode.toInt());
            } else {
                httpStatus = HTTPInvocationDefinition::UNDEFINED;
                result << Result::reasonFromDesc("Http response code is not valid");
            }

            invocationStatus = Invocation::ERROR;

            // get possible return data (detailed error info)
            // responseErrorData = qreply->readAll();
            result << InvocationErrors::CONNECTION_FAILED
                   << Result::Meta("qnetworkreply_error_string", qreply->errorString());

            QNetworkReply::NetworkError err = qreply->error();
            result << NetworkErrorReasons::from(err);

            //result << Result::reasonFromCode("QNETWORKREPLY_ERROR", qreply->errorString());
            emit q->finishedError(q);
        }
        else
        {
            // we have all data
            // (TODO: is it so that readRead() is invoked before?)

            if (outputFile.isOpen())
                outputFile.close();

            invocationStatus = Invocation::FINISHED;
            emit q->finishedOK(q);
        }

        qreply->deleteLater();
        qreply = nullptr;
    }
};


DownloadStreamInvocationImpl::DownloadStreamInvocationImpl(InvocationFactoryImpl* factory, QObject* parent) :
    DownloadStreamInvocation(parent),
    _d(new DownloadStreamInvocationImplPrivate(this, factory))
{
    // TODO: impl timeout
}

DownloadStreamInvocationImpl::~DownloadStreamInvocationImpl()
{
    if (_d->invocationStatus == Invocation::ONGOING)
        _d->qreply->abort();
}

void DownloadStreamInvocationImpl::defineGetOperation(const QString& invocationPath)
{
    _d->def.setHttpOperation(HTTPInvocationDefinition::GET);
    _d->def.setInvocationPath(invocationPath);
}

void DownloadStreamInvocationImpl::definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc)
{
    _d->def.setHttpOperation(HTTPInvocationDefinition::POST);
    _d->def.setInvocationPath(invocationPath);

    _d->postData = jsondoc.toJson();
}

void DownloadStreamInvocationImpl::defineParameter(const QString &paramName, const QString &paramValue)
{
    _d->params[paramName] = paramValue;
}

void DownloadStreamInvocationImpl::execute()
{
   if (_d->outputFilePath.isEmpty()) {
       _d->invocationStatus = Invocation::INVALID_INVOCATION;
       _d->result << InvocationErrors::INVOCATION_INVALID
                  << Result::reasonFromDesc("No output file path was defined");
       emit finishedError(this);
   } else {
       _d->doOperation();
   }
}

void DownloadStreamInvocationImpl::abort()
{
    if (_d->qreply)
        _d->qreply->abort();
}

Invocation::InvocationStatus DownloadStreamInvocationImpl::statusCode() const
{
    return _d->invocationStatus;
}

HTTPInvocationDefinition::Status DownloadStreamInvocationImpl::responseHttpStatusCode() const
{
    return _d->httpStatus;
}

Result DownloadStreamInvocationImpl::result() const
{
    return _d->result;
}

int DownloadStreamInvocationImpl::progressPercentage() const
{
    // guard against division by zero if for some reason total bytes not know
    if (_d->bytesTotal == 0)
        return 0;

    int percentage = _d->bytesTotal != -1 ? _d->bytesReceived * 100 / _d->bytesTotal : 0;
    return percentage;
}

void GBerry::DownloadStreamInvocationImpl::setOutputFilePath(const QString& outputFilePath)
{
    _d->outputFilePath = outputFilePath;
    _d->outputFile.setFileName(outputFilePath);
}

} // eon
