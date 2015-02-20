#ifndef COMMCHANNELSERVER_H
#define COMMCHANNELSERVER_H

#include <QObject>

class QTcpServer;

class CommChannelServer : public QObject
{
    Q_OBJECT
public:
    explicit CommChannelServer(int port, QObject *parent = 0);
    ~CommChannelServer();

    void open();
    void close();

signals:
    void connected();
    void disconnected();

    void opened(int channelID);
    void received(int channelID, QByteArray& msg);

public slots:
    void newConnection();

    // TODO: responsibility to read data in prefix lengths (first decoding)
    //        -> forwards messages to MessageHandler
    // TODO: possible to write data to open channel

private:
    QTcpServer* _server;
    int _port;
};

#endif // COMMCHANNELSERVER_H
