#include "connectionmanager.h"

#include "commtcpserver.h"
#include "serverchannelmanager.h"
#include "serversidecontrolchannel.h"
#include "connectiongatekeeper.h"
#include "applicationregistry.h"

#define LOG_AREA "ConnectionManager"
#include "log/log.h"

using namespace GBerry::Console::Server;

ConnectionManager::ConnectionManager(CommTcpServer* tcpServer,
                                     ApplicationRegistry* appRegistry,
                                     ServerChannelManager* channelManager,
                                     QObject *parent) :
    QObject(parent),
    _tcpServer(tcpServer),
    _applicationRegistry(appRegistry),
    _channelManager(channelManager)
{
}

ConnectionManager::~ConnectionManager()
{
}

bool ConnectionManager::activeConnection()
{
    return _channelManager->hasActiveConnection();
}


void ConnectionManager::applicationConnected(int connectionId)
{
    ConnectionGateKeeper* gatekeeper = new ConnectionGateKeeper(connectionId, _applicationRegistry, this);
    connect(gatekeeper, &ConnectionGateKeeper::connectionValidated, this, &ConnectionManager::onConnectionValidationOK);
    connect(gatekeeper, &ConnectionGateKeeper::connectionDiscarded, this, &ConnectionManager::onConnectionValidationFailed);
    connect(gatekeeper, &ConnectionGateKeeper::outgoingMessage, this, &ConnectionManager::onOutgoingMessageFromGateKeeper);

    _gatekeepers[connectionId] = gatekeeper;
    gatekeeper->validate();
}

void ConnectionManager::applicationDisconnected(int connectionId)
{
    _tcpServer->closeConnection(connectionId);
    _channelManager->applicationClosed(connectionId);
    _applicationRegistry->removeLink(connectionId);
}

void ConnectionManager::incomingMessage(int connectionId, int channelId, const QByteArray msg)
{
    DEBUG("incomingMessage(): cid=" << channelId);
    if (_gatekeepers.contains(connectionId)) {
        _gatekeepers[connectionId]->incomingMessage(msg);
    } else {
        _channelManager->processMessageFromSouth(connectionId, channelId, msg);
    }
}

void ConnectionManager::outgoingMessageFromChannel(int connectionId, int channelId, const QByteArray msg)
{
    _tcpServer->write(connectionId, channelId, msg);
}

void ConnectionManager::onOutgoingMessageFromGateKeeper(int connectionId, const QByteArray &msg)
{
    _tcpServer->write(connectionId, ServerSideControlChannel::CHANNEL_ID, msg);
}

void ConnectionManager::onConnectionValidationOK(int connectionId)
{
    if (!_gatekeepers.contains(connectionId)) {
        ERROR("Got signal from ConnectionGatekeeper that is not registered. Ignoring it.");
        return;
    }
    ConnectionGateKeeper* gatekeeper = _gatekeepers[connectionId];
    _gatekeepers.remove(connectionId);

    _applicationRegistry->createLink(connectionId, gatekeeper->validatedApplicationId());

    _channelManager->openControlChannel(connectionId);

    // latest will always be active
    _channelManager->activateConnection(connectionId);

    // TODO: there is state between app deactivate and active -> players should have waiting state

    // if no applicationId then empty string

    emit applicationConnectionValidated(gatekeeper->validatedApplicationId());

    // guard is no longer needed
    gatekeeper->deleteLater();
}


void ConnectionManager::onConnectionValidationFailed(int connectionId)
{
    if (!_gatekeepers.contains(connectionId)) {
        ERROR("Got signal from ConnectionGatekeeper that is not registered. Ignoring it.");
        return;
    }
    ConnectionGateKeeper* gatekeeper = _gatekeepers[connectionId];
    _gatekeepers.remove(connectionId);

    _tcpServer->closeConnection(connectionId); // note that we will get disconnect signal from this

    // no need to close ChannelManagers as we haven't opened them yet

    gatekeeper->deleteLater();
}
