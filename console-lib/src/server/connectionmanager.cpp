#include "connectionmanager.h"

#include "commtcpserver.h"
#include "serversidechannelmanager.h"
#include "serversidecontrolchannel.h"


ConnectionManager::ConnectionManager(CommTcpServer* tcpServer,
                                     ServerSideChannelManager* channelManager,
                                     ServerSideControlChannel* controlChannel,
                                     QObject *parent) :
    QObject(parent),
    _tcpServer(tcpServer),
    _channelManager(channelManager),
    _controlChannel(controlChannel),
    _activeConnectionId(-1)
{

}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::closeCurrentApplication()
{
    if (_activeConnectionId > -1)
    {
        _tcpServer->closeConnection(_activeConnectionId); // note that we will get disconnect signal from this
        _channelManager->applicationClosed();
        _activeConnectionId = -1;
    }
}

void ConnectionManager::applicationConnected(int connectionId)
{
    if (_activeConnectionId > -1)
    {
        // there was a previous active connection
        closeCurrentApplication();

        // TODO: now just closing, maybe later something more clever, but
        //       need to take care of state ChannelManager, as now it tried to
        //       be independent of connection (only one state for all connections)
    }

    _activeConnectionId = connectionId;

    // first we should get ping through to verify we can talk
    // (then ChannelManager takes care of opening channels)
    _controlChannel->ping();
}

void ConnectionManager::applicationDisconnected(int connectionId)
{
    closeCurrentApplication();
}

bool ConnectionManager::activeConnection()
{
    return _activeConnectionId != -1;
}

void ConnectionManager::incomingMessage(int connectionId, int channelId, const QByteArray &msg)
{
    if (connectionId != _activeConnectionId)
    {
        // record keeping is not in sync! Suspicious
        qCritical() << "[ConnectionManager] Discarding message because connectioId is different from active connection:"
                    << "active =" << _activeConnectionId
                    << ", got =" << connectionId;

        return; // skip message
    }

    _channelManager->processMessage(channelId, msg);
}

void ConnectionManager::outgoingMessageFromChannel(int channelId, const QByteArray &msg)
{
    if (activeConnection())
    {
        _tcpServer->write(_activeConnectionId, channelId, msg);
    }
    else
    {
        qWarning() << "Message from channel while no active connection:"
                   << "channelId =" << channelId;
    }
}

// TODO: we can handle pinging here (timer)
