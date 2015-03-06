#include "restinvocationimpl.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>


RESTInvocationImpl::RESTInvocationImpl(RESTInvocationFactoryImpl* factory, QObject* parent) :
    RESTInvocation(parent),
    _factory(factory),
    _reply(NULL)
{
    this->setProperty("timeout", DEFAULT_TIMEOUT_MS);

    // TODO: imlp timeout
}

RESTInvocationImpl::~RESTInvocationImpl()
{

}

void RESTInvocationImpl::get(QString invocationPath)
{
    get(_factory->buildUrl(invocationPath));
}

void RESTInvocationImpl::get(QUrl url)
{
    // TODO: guard against having multiple queries ongoing (buffering them?), only one per this instance

    // store for later use (e.g. redirects)
    _url = url;

    // TODO: is there signal for error situation
    _reply = _factory->getQNetworkAccessManager()->get(QNetworkRequest(_url));

    qDebug("### CONNECTING");
    connect(_reply, &QNetworkReply::finished,
            this,   &RESTInvocationImpl::httpFinished);

    _invocationStatus = RESTInvocation::ONGOING;
}

void RESTInvocationImpl::post(QString invocationPath, QJsonDocument jsondoc)
{
    post(_factory->buildUrl(invocationPath), jsondoc);
}

void RESTInvocationImpl::post(QUrl url, QJsonDocument jsondoc)
{
    _url = url;
    QByteArray body = jsondoc.toJson();
    QNetworkRequest req(_url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    _reply = _factory->getQNetworkAccessManager()->post(req, body);

    qDebug("### CONNECTING POST");
    connect(_reply, &QNetworkReply::finished,
            this,   &RESTInvocationImpl::httpFinished);

    _invocationStatus = RESTInvocation::ONGOING;
}

RESTInvocation::InvocationStatus RESTInvocationImpl::statusCode() const
{
    return _invocationStatus;
}

RESTInvocation::HttpStatus RESTInvocationImpl::responseHttpStatusCode() const
{
    return _httpStatus;
}

bool RESTInvocationImpl::responseAvailable() const
{
    return _invocationStatus == RESTInvocation::RESPONSE_RECEIVED;
}

QByteArray RESTInvocationImpl::responseByteData() const
{
    // TODO: just fake impl
    return _data.toLatin1();
}

QString RESTInvocationImpl::responseString() const
{
    return _data;
}

void RESTInvocationImpl::httpFinished()
{
    qDebug("### httpFinished");
    QVariant redirectionTarget = _reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!redirectionTarget.isNull())
    {
        QUrl newUrl = _url.resolved(redirectionTarget.toUrl());
        qDebug() << "Handled redirect to: " << newUrl;
        _reply->deleteLater();
        _reply = NULL;
        get(newUrl); // TODO: operation might not be get()
        return;
    }
    else if (_reply->error())
    {
        // TODO: logging
        qDebug() << "HTTP ERROR: " << _reply->errorString();
        // TODO: error code
        _httpStatus = RESTInvocation::UNDEFINED;
        _invocationStatus = RESTInvocation::RESPONSE_RECEIVED;

        emit finishedError(this);
    }
    else
    {
        // we have data
        _data = _reply->readAll();
        QVariant statusCode = _reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if (statusCode.isValid())
        {
            // TODO: handle more codes!
            switch (statusCode.toInt())
            {
            case 200:
                _httpStatus = RESTInvocation::OK_200;
                break;
            default:
                _httpStatus = RESTInvocation::UNDEFINED;
            }
        }

        _invocationStatus = RESTInvocation::RESPONSE_RECEIVED;
        // TODO: better logging
        qDebug() << "DATA:" << _data;
        emit finishedOK(this);
    }

    _reply->deleteLater();
    _reply = NULL;
}
