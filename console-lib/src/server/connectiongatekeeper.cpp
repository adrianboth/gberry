#include "connectiongatekeeper.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "applicationregistry.h"

#define LOG_AREA "ConnectionGateKeeper"
#include "log/log.h"

namespace GBerry {
namespace Console {
namespace Server {

class ConnectionGateKeeperPrivate
{
public:
    ConnectionGateKeeperPrivate(int connectionId_, ApplicationRegistry* registry) :
        connectionId(connectionId_),
        appRegistry(registry),
        validatedApplicationId("") {}

    int connectionId;
    ApplicationRegistry* appRegistry;
    QTimer timer;
    QString validatedApplicationId;
};

ConnectionGateKeeper::ConnectionGateKeeper(
        int connectionId,
        ApplicationRegistry* appRegistry,
        QObject *parent) :
    QObject(parent),
    _d(new ConnectionGateKeeperPrivate(connectionId, appRegistry))
{
    _d->timer.setSingleShot(true);
    _d->timer.setInterval(5000); // 5s TODO: should be read from a property
    connect(&_d->timer, &QTimer::timeout, this, &ConnectionGateKeeper::waitTimeout);
}

ConnectionGateKeeper::~ConnectionGateKeeper()
{
    _d->timer.stop();
}

void ConnectionGateKeeper::validate()
{
    QJsonObject json;
    json["command"] = "Ping";
    QJsonDocument jsonDoc(json);
    emit outgoingMessage(_d->connectionId, jsonDoc.toJson());
    _d->timer.start();
}

void ConnectionGateKeeper::incomingMessage(const QByteArray &msg)
{
    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(msg, &error));
    if (error.error != QJsonParseError::NoError) {
        ERROR("Failed to parse json message:" << msg);
        return;
    }

    if (!_d->timer.isActive()) {
        ERROR("Got message eventhough timer is not anymore running");
        return;
    }

    QJsonObject json(doc.object());

    // TODO: should be 'response'
    if (json.contains("command") && json["command"] == "PingReply")
    {
        if (json.contains("application_code")) {
            QString code = json["application_code"].toString();

            // if not found then not valid
            QString appId = _d->appRegistry->matchCodeToApplication(code);
            if (appId.isEmpty()) {
                emit connectionDiscarded(_d->connectionId);
            } else {
                _d->validatedApplicationId = appId;
                emit connectionValidated(_d->connectionId);
            }
        } else {
            // no code, accept as it is (others may still deny it)
            emit connectionValidated(_d->connectionId); // anonymous connection
        }
        _d->timer.stop();

    } else {
        // invalid message, wait still for ok message
    }

}

void ConnectionGateKeeper::waitTimeout()
{
    // ping reply hasn't arrived -> discard connection
    emit connectionDiscarded(_d->connectionId);
}

QString ConnectionGateKeeper::validatedApplicationId()
{
    return _d->validatedApplicationId;
}

}}} // eon
