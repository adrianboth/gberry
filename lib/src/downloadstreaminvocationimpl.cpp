#include "downloadstreaminvocationimpl.h"

#include <QUrl>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDataStream>


#include "httpinvocationdefinition.h"

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
    QString errorString;
    // TODO: encoding,  content-type
    QNetworkReply* qreply{nullptr};
    qint64 totalReadBytes{0};

    QUrl url;
    HTTPInvocationDefinition def;

    QByteArray postData;
    QString outputFilePath;
    QFile outputFile;
    char* readBuffer;
    qint64 bytesReceived;
    qint64 bytesTotal;


    void doOperation() {
        doOperation(invocationFactory->buildUrl(def.invocationPath()));
    }

    void doOperation(QUrl url) {
        switch (def.httpOperation()) {
            case HTTPInvocationDefinition::GET:
                get(url);
                break;
            case HTTPInvocationDefinition::POST:
                post(url);
                break;
            case HTTPInvocationDefinition::NOT_DEFINED:
                // TODO: error
                break;
            default:
                // TODO: unknown -> error
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

        QObject::connect(qreply, &QNetworkReply::downloadProgress,
                          [this] (qint64 bytesReceived, qint64 bytesTotal) {
            this->onQNetworkReplyDownloadProgress(bytesReceived, bytesTotal);
        });

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
        bool firstRead = invocationStatus != Invocation::RESPONSE_RECEIVED;
        invocationStatus = Invocation::RESPONSE_RECEIVED;

        QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if (statusCode.isValid())
        {
            httpStatus = HTTPInvocationDefinition::resolveHttpStatus(statusCode.toInt());
        } else {
            // TODO: what to do, when no code?
            invocationStatus = Invocation::ERROR;
            return;
        }

        if (httpStatus != HTTPInvocationDefinition::OK_200) {
            invocationStatus = Invocation::ERROR;
            // nothing to save to file
            DEBUG("Error no data to save to file");
            return;
        }

        // is target open?
        if (!outputFile.isOpen()) {
            if (!outputFile.open(QIODevice::WriteOnly)) {
                q->abort();
                errorString = "Open output file failed"; // TODO: how to pass more accurate information
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
            QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
            if (statusCode.isValid()) {
                httpStatus = HTTPInvocationDefinition::resolveHttpStatus(statusCode.toInt());
            } else {
                httpStatus = HTTPInvocationDefinition::UNDEFINED;
            }

            invocationStatus = Invocation::ERROR;

            // get possible return data (detailed error info)
            responseErrorData = qreply->readAll();
            errorString = qreply->errorString();
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

void DownloadStreamInvocationImpl::execute()
{
   if (_d->outputFilePath.isEmpty()) {
       _d->errorString = "No output file path was defined";
       _d->invocationStatus = Invocation::INVALID_INVOCATION;
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

QString DownloadStreamInvocationImpl::errorString() const
{
    return _d->errorString;
}

int DownloadStreamInvocationImpl::progressPercentage() const
{
    int percentage = _d->bytesTotal != -1 ? _d->bytesReceived * 100 / _d->bytesTotal : 0;
    return percentage;
}


void GBerry::DownloadStreamInvocationImpl::setOutputFilePath(QString outputFilePath)
{
    _d->outputFilePath = outputFilePath;
    _d->outputFile.setFileName(outputFilePath);
}


} // eon
