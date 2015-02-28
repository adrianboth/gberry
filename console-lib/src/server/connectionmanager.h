#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QByteArray>

class CommTcpServer;
class ServerSideChannelManager;
class ServerSideControlChannel;


class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(CommTcpServer* tcpServer,
                               ServerSideChannelManager* channelManager,
                               ServerSideControlChannel* controlChannel,
                               QObject *parent = 0);
    ~ConnectionManager();

    void closeCurrentApplication();

    // true if active connection exists
    bool activeConnection();

signals:    

public slots:
    void applicationConnected(int connectionId);
    void applicationDisconnected(int connectionId);
    void pingOK();
    void incomingMessage(int connectionId, int channelId, const QByteArray msg);
    void outgoingMessageFromChannel(int channelId, const QByteArray msg);

private:
    CommTcpServer* _tcpServer;
    ServerSideChannelManager* _channelManager;
    ServerSideControlChannel* _controlChannel;
    int _activeConnectionId;
};

#endif // CONNECTIONMANAGER_H
