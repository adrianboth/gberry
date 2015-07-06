#include "restinvocationimpl.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "httpinvocationdefinition.h"

#define LOG_AREA "RESTInvocationImpl"
#include "log/log.h"


class RESTInvocationImplPrivate
{
public:
    RESTInvocationImplPrivate(RESTInvocation* q_ ,InvocationFactoryImpl* factory) :
        q(q_),
        invocationFactory(factory) {}

    RESTInvocation* q;
    InvocationFactoryImpl* invocationFactory;
    RESTInvocation::InvocationStatus invocationStatus{RESTInvocation::NOT_STARTED};
    HTTPInvocationDefinition::Status httpStatus{HTTPInvocationDefinition::UNDEFINED};
    QString replyData;
    // TODO: encoding,  content-type
    QNetworkReply* qreply{nullptr};

    HTTPInvocationDefinition def;
    QUrl url;
    QByteArray postData;

    void doOperation() {
        doOperation(invocationFactory->buildUrl(def.invocationPath()));
    }

    void doOperation(QUrl url) {
        switch (def.httpOperation()) {
            case HTTPInvocationDefinition::NOT_DEFINED:
                // TODO: error
                break;
            case HTTPInvocationDefinition::GET:
                get(url);
                break;

            case HTTPInvocationDefinition::POST:
                post(url);
                break;

            default:
                ERROR("Unsupported http operation");
                // TODO: error
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
                [this] () { this->httpFinished(); });

        invocationStatus = RESTInvocation::ONGOING;
    }

    void post(QUrl url) {
        // store for redirects
        this->url = url;

        DEBUG("Executing POST operation");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        qreply = invocationFactory->getQNetworkAccessManager()->post(req, postData);

        QObject::connect(qreply, &QNetworkReply::finished,
                [this] () { this->httpFinished(); });

        invocationStatus = RESTInvocation::ONGOING;
    }

    void httpFinished() {
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
            // TODO: error code
            httpStatus = HTTPInvocationDefinition::UNDEFINED;
            invocationStatus = Invocation::RESPONSE_RECEIVED;

            // get possible return data
            replyData = qreply->readAll();

            emit q->finishedError(q);
        }
        else
        {
            // we have data
            replyData = qreply->readAll();
            QVariant statusCode = qreply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
            if (statusCode.isValid())
            {
                // TODO: handle more codes!
                switch (statusCode.toInt())
                {
                case 200:
                    httpStatus = HTTPInvocationDefinition::OK_200;
                    break;
                default:
                    httpStatus = HTTPInvocationDefinition::UNDEFINED;
                }
            }

            invocationStatus = Invocation::RESPONSE_RECEIVED;
            TRACE("DATA:" << replyData);
            emit q->finishedOK(q);
        }

        qreply->deleteLater();
        qreply = nullptr;
    }
};

RESTInvocationImpl::RESTInvocationImpl(InvocationFactoryImpl* factory, QObject* parent) :
    RESTInvocation(parent),
    _d(new RESTInvocationImplPrivate(this, factory))
{
    this->setProperty("timeout", DEFAULT_TIMEOUT_MS);

    // TODO: impl timeout
}

RESTInvocationImpl::~RESTInvocationImpl()
{
    if (_d->invocationStatus == RESTInvocation::ONGOING)
        _d->qreply->abort();
}

void RESTInvocationImpl::defineGetOperation(const QString& invocationPath)
{
    _d->def.setHttpOperation(HTTPInvocationDefinition::GET);
    _d->def.setInvocationPath(invocationPath);
}

void RESTInvocationImpl::definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc)
{
    _d->def.setHttpOperation(HTTPInvocationDefinition::POST);
    _d->def.setInvocationPath(invocationPath);
    _d->postData = jsondoc.toJson();
}

void RESTInvocationImpl::execute()
{
    _d->doOperation();
}

void RESTInvocationImpl::abort()
{
    if (_d->qreply)
        _d->qreply->abort();
}

Invocation::InvocationStatus RESTInvocationImpl::statusCode() const
{
    return _d->invocationStatus;
}

HTTPInvocationDefinition::Status RESTInvocationImpl::responseHttpStatusCode() const
{
    return _d->httpStatus;
}

bool RESTInvocationImpl::responseAvailable() const
{
    return _d->invocationStatus == RESTInvocation::RESPONSE_RECEIVED;
}

QByteArray RESTInvocationImpl::responseByteData() const
{
    // TODO: just fake impl
    return _d->replyData.toLatin1();
}

QString RESTInvocationImpl::responseString() const
{
    return _d->replyData;
}

QString RESTInvocationImpl::errorString() const
{
    if (_d->qreply)
        return _d->qreply->errorString();
    else
        return "";
}
