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
 
 #include "restinvocationimpl.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "httpinvocationdefinition.h"
#include "networkerrorreasons.h"

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
    Result result;
    QMap<QString, QString> params;

    void doOperation() {
        doOperation(invocationFactory->buildUrl(def.invocationPath(), params));
    }

    void doOperation(QUrl url) {
        result << Result::Meta("url", url.toString());

        switch (def.httpOperation()) {
            case HTTPInvocationDefinition::NOT_DEFINED:
                result << InvocationErrors::INVOCATION_INVALID
                       << Result::reasonFromDesc("Http operation not defined");
                emit q->finishedError(q);
                break;

            case HTTPInvocationDefinition::GET:
                result << Result::Meta("http_operation", "GET");
                get(url);
                break;

            case HTTPInvocationDefinition::POST:
                result << Result::Meta("http_operation", "POST");
                post(url);
                break;

            default:
                ERROR("Unsupported http operation");
                result << InvocationErrors::INVOCATION_INVALID
                       << Result::reasonFromDesc("Unknown http operation");
                emit q->finishedError(q);
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

        // TODO: behind debug flag
        DEBUG("Executing POST operation:" << url.toString() << ", data:" << QString(postData));

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
            WARN("HTTP ERROR:" << qreply->error() << qreply->errorString());

            httpStatus = HTTPInvocationDefinition::UNDEFINED;
            invocationStatus = Invocation::ERROR;
            result << InvocationErrors::CONNECTION_FAILED
                   << Result::Meta("qnetworkreply_error_string", qreply->errorString())
                   << Result::Meta("qnetworkreply_error_number", qreply->error());

            QNetworkReply::NetworkError err = qreply->error();
            result << NetworkErrorReasons::from(err, qreply->errorString());

            // get possible return data
            replyData = qreply->readAll();
            if (!replyData.isEmpty())
                result << Result::Meta("response_data", replyData);

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
            } else {
                result << InvocationErrors::INVOCATION_FAILED
                       << Result::reasonFromDesc("Invalid http status code");
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

void RESTInvocationImpl::defineParameter(const QString &paramName, const QString &paramValue)
{
    _d->params[paramName] = paramValue;
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

Result RESTInvocationImpl::result() const
{
    return _d->result;
}
