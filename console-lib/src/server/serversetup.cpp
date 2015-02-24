#include "serversetup.h"

ServerSetup::ServerSetup(QObject* parent)
    : QObject(parent), tcpServer(7777)
{
    // we discard connectionId, in future it might have meaning
    connect(&tcpServer, &CommTcpServer::received,
            [this] (int connectionId, int channelId, const QByteArray& msg) {
        Q_UNUSED(connectionId);
        this->channelManager.processMessage(channelId, msg);
    });

    connect(&tcpServer, &CommTcpServer::disconnected,
            [this] (int connectionId) {
        Q_UNUSED(connectionId);
        this->channelManager.applicationClosed();
    });

    channelManager.registerHandler(&controlChannel);

}

ServerSetup::~ServerSetup()
{
    tcpServer.close();
}

void ServerSetup::start()
{
    tcpServer.open();
}
