#include "downloadstreaminvocationimpl.h"

#include <QUrl>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDataStream>


#include "restinvocationdefinition.h"

#define LOG_AREA "DownloadStreamInvocationImpl"
#include "log/log.h"

namespace {
    const int READ_BUFFER_SIZE = 10000;
}

namespace GBerry {

class DownloadStreamInvocationImplPrivate
{
public:
    DownloadStreamInvocationImplPrivate(DownloadStreamInvocation* q_ ,RESTInvocationFactoryImpl* factory) :
        q(q_),
        invocationFactory(factory) {
        readBuffer = new char[READ_BUFFER_SIZE]; // TODO: somehow tunable parameter
    }
    ~DownloadStreamInvocationImplPrivate() {
        delete readBuffer;
    }

    DownloadStreamInvocation* q;
    RESTInvocationFactoryImpl* invocationFactory;
    Invocation::InvocationStatus invocationStatus{Invocation::NOT_STARTED};
    RESTInvocationDefinition::HttpStatus httpStatus{RESTInvocationDefinition::UNDEFINED};
    QString responseErrorData;
    QString errorString;
    // TODO: encoding,  content-type
    QNetworkReply* qreply{nullptr};

    QUrl url;
    RESTInvocationDefinition def;

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
            case RESTInvocationDefinition::NOT_DEFINED:
                // TODO: error
                break;
            case RESTInvocationDefinition::GET:
                get(url);
            case RESTInvocationDefinition::POST:
                post(url);
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
                [this] () { this->onQNetworkReplyFinished(); });

        QObject::connect(qreply, &QNetworkReply::readyRead,
                [this] () { this->onQNetworkReplyReadyRead(); });

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

        invocationStatus = Invocation::ONGOING;
    }

    void onQNetworkReplyReadyRead() {
        invocationStatus = Invocation::RESPONSE_RECEIVED;

        QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if (statusCode.isValid())
        {
            // TODO: handle more codes!
            switch (statusCode.toInt())
            {
            case 200:
                httpStatus = RESTInvocationDefinition::OK_200;
                break;
            default:
                httpStatus = RESTInvocationDefinition::UNDEFINED;
                // TODO: there is an error -> how to abort
                DEBUG("GOT ERROR");
                invocationStatus = Invocation::ERROR;
                return;

            }
        } else {
            // TODO: what to do, when no code?
            invocationStatus = Invocation::ERROR;
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

        int readBytes = 0;
        while ( (readBytes = outputFile.read(readBuffer, READ_BUFFER_SIZE) ) > 0) {
            outputFile.write(readBuffer, readBytes);
        }
    }

    void onQNetworkReplyDownloadProgress(qint64 bytesReceived_, qint64 bytesTotal_) {
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
            httpStatus = RESTInvocationDefinition::UNDEFINED;
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


DownloadStreamInvocationImpl::DownloadStreamInvocationImpl(RESTInvocationFactoryImpl* factory, QObject* parent) :
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
    _d->def.setHttpOperation(RESTInvocationDefinition::GET);
    _d->def.setInvocationPath(invocationPath);
}

void DownloadStreamInvocationImpl::definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc)
{
    _d->def.setHttpOperation(RESTInvocationDefinition::POST);
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

RESTInvocationDefinition::HttpStatus DownloadStreamInvocationImpl::responseHttpStatusCode() const
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
