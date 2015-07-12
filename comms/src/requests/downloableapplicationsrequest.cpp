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

QList<QSharedPointer<Application> > DownloadableApplicationsRequest::receivedApplications() const
{
    return _receivedApplications;
}

Invocation* DownloadableApplicationsRequest::processPrepare(InvocationFactory *factory)
{
    RESTInvocation* inv = factory->newRESTInvocation();
    // TODO: hard coded token
    inv->defineGetOperation("/application/list?user_token=818ec382-5a10-484a-a22f-fc90015643cb");

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
        this->processErrorResponse(Request::ERROR_INVALID_JSON_RESPONSE, invocation);
        return;
    }
    // TODO: we should validate that there is really array
    QJsonArray arr = json["applications"].toArray();
    foreach (QJsonValue jv, arr) {
        QJsonObject j = jv.toObject();
        ApplicationMeta* meta = new ApplicationMeta;
        meta->setApplicationId(j["name"].toString()); // TODO: name is not really the id
        meta->setVersion(j["version"].toString());
        meta->setName(j["name"].toString());
        meta->setDescription(j["description"].toString());
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

void DownloadableApplicationsRequest::processErrorResponse(Request::Error error, Invocation *invocation)
{

    DEBUG("processErrorResponse()");
    // TODO: actual action
    _command->processRequestErrorResponse(this);
    DEBUG("processErrorResponse(): done");

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    }

}


} // eon
