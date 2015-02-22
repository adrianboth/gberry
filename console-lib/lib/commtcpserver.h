#ifndef COMMCHANNELSERVER_H
#define COMMCHANNELSERVER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <QAbstractSocket>


class QTcpServer;
class QTcpSocket;
class CommTcpServer;
class TcpConnection;



class CommTcpServer : public QObject
{
    Q_OBJECT

public:
    explicit CommTcpServer(int port, QObject *parent = 0);
    ~CommTcpServer();

    void open();
    void close();

    void closeConnection2(int id);
    void write(int connectionId, QByteArray& data);

signals:
    void connected(int id); // TODO: when used
    void disconnected(int channelId);

    // TODO: change parameter name
    void received(int channelID, QByteArray msg);

public slots:
    void newConnection();

    // TODO: responsibility to read data in prefix lengths (first decoding)
    //        -> forwards messages to MessageHandler
    // TODO: possible to write data to open channel

private:
    QTcpServer* _server;
    int _port;
    QMap<int, TcpConnection*> _connections;
    int _connectionIdCount;
};


// -- private

class TcpConnection : public QObject
{
    Q_OBJECT

public:
    TcpConnection(int id, QTcpSocket* s, CommTcpServer* ccs);

    int id;
    QTcpSocket* socket;

    CommTcpServer* _comm;

public slots:
    void disconnect();
    void read();
    void error(QAbstractSocket::SocketError);

private:

    quint32 _blockSize;


};


#endif // COMMCHANNELSERVER_H
