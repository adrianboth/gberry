#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QMap>

class CommTcpServer;
class ServerChannelManager;
class ServerSideControlChannel;

namespace GBerry {
namespace Console {
namespace Server {
    class ApplicationRegistry;
    class ConnectionGateKeeper;
}}}

using namespace GBerry::Console::Server;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(CommTcpServer* tcpServer,
                               ApplicationRegistry* appRegistry,
                               ServerChannelManager* channelManager,
                               QObject *parent = 0);
    ~ConnectionManager();

    void closeCurrentApplication();

    // true if active connection exists
    bool activeConnection();

signals:    
    void applicationConnectionValidated(QString appId);

public slots:
    void applicationConnected(int connectionId);
    void applicationDisconnected(int connectionId);
    void incomingMessage(int connectionId, int channelId, const QByteArray msg);
    void outgoingMessageFromChannel(int connectionId, int channelId, const QByteArray msg);

    void onConnectionValidationOK(int connectionId);
    void onConnectionValidationFailed(int connectionId);
    void onOutgoingMessageFromGateKeeper(int connectionId, const QByteArray& msg);

private:
    CommTcpServer* _tcpServer;
    ApplicationRegistry* _applicationRegistry;
    ServerChannelManager* _channelManager;
    QMap<int, ConnectionGateKeeper*> _gatekeepers;
};

#endif // CONNECTIONMANAGER_H
