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
 
 #include "invocation.h"
#include "restinvocation.h"
#include "downloableapplicationsrequest.h"
#include "commands/querydownloadableapplicationscommand.h"
#include "restinvocationimpl.h"
#include "application.h"
#include "server/application/applicationmeta.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

#define LOG_AREA "DownloadableApplicationsRequest"
#include "log/log.h"

namespace GBerry {

DownloadableApplicationsRequest::DownloadableApplicationsRequest(
        QueryDownloadableApplicationsCommand* command) :
    _command(command)
{
}

DownloadableApplicationsRequest::~DownloadableApplicationsRequest()
{
}

void DownloadableApplicationsRequest::setUserToken(int playerId, const QString &userToken)
{
    _playerId = playerId;
    _userToken = userToken;
}

bool DownloadableApplicationsRequest::playerDefined() const
{
    return _playerId != -1;
}

int DownloadableApplicationsRequest::playerId() const
{
    return _playerId;
}

QList<QSharedPointer<Application> > DownloadableApplicationsRequest::receivedApplications() const
{
    return _receivedApplications;
}

void DownloadableApplicationsRequest::fillInErrorDetails(Result &res)
{
    res << Result::Meta("player_id", _playerId);
    res << Result::Meta("user_token", _userToken.isEmpty() ? "<empty>" : "<defined>");
}

Invocation* DownloadableApplicationsRequest::processPrepare(InvocationFactory *factory)
{
    RESTInvocation* inv = factory->newRESTInvocation();
    inv->defineGetOperation("/application/list");
    if (_userToken.isEmpty()) {
        inv->defineParameter("type", "free");
    } else {
        inv->defineParameter("user_token", _userToken);
    }

    return inv;
}

void DownloadableApplicationsRequest::processOkResponse(Invocation *invocation)
{
    RESTInvocation* inv = qobject_cast<RESTInvocation*>(invocation);

    // TODO: parse response (json)
    /*
     {"applications": [{"name": "TestOffering", "version": "1.0", "image_url": "http://localhost:8000/store/store/media/admin__TestOffering__1.0/testpic_gb.png"},
                       {"name": "TestOffering3", "version": "2.0", "image_url": "http://localhost:8000/store/store/media/admin__TestOffering3__2.0/testpic_gb.png"},
                       {"name": "Test Offering 2", "version": "1.1", "image_url": "http://localhost:8000/store/store/media/admin__Test Offering 2__1.1/testpic2_gb.png"}]}
     */
    QJsonParseError error;
    QByteArray data(inv->responseString().toLatin1());
    QJsonObject json = QJsonDocument::fromJson(data, &error).object();

    if (error.error != QJsonParseError::NoError) {
        // TODO: handle error
        this->processErrorResponse(RequestErrors::INVALID_JSON_RESPONSE, invocation);
        return;
    }
    // TODO: we should validate that there is really array
    QJsonArray arr = json["applications"].toArray();
    foreach (QJsonValue jv, arr) {
        QJsonObject j = jv.toObject();
        ApplicationMeta* meta = new ApplicationMeta;
        meta->setApplicationId(j["id"].toString());
        meta->setVersion(j["version"].toString());
        meta->setName(j["name"].toString());
        meta->setDescription(j["description"].toString());
        meta->setIsFree(j["is_free"].toBool());
        QSharedPointer<Application> app(new Application(QSharedPointer<ApplicationMeta>(meta)));
        _receivedApplications << app;

        // TODO: how image url ?? do you need token there??
    }

    DEBUG("processRequestOkResponse()");
    _command->processRequestOkResponse(this);

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    };
}

void DownloadableApplicationsRequest::processErrorResponse(const Result& res, Invocation *invocation)
{
    Q_UNUSED(invocation);
    DEBUG("processErrorResponse()");
    // TODO: actual action
    _command->processRequestErrorResponse(this, res);
    DEBUG("processErrorResponse(): done");

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    }

}

} // eon
