#ifndef COMMCHANNELSERVER_H
#define COMMCHANNELSERVER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <QAbstractSocket>


class QTcpServer;
class QTcpSocket;
class TcpConnection;
class MessageReader;

class CommTcpServer : public QObject
{
    Q_OBJECT

public:
    explicit CommTcpServer(int port, QObject *parent = 0);
    ~CommTcpServer();

    void open();
    void close();

    void closeConnection2(int id);
    void write(int connectionId, const QByteArray& msg);

signals:
    void connected(int connectionId); // TODO: when used
    void disconnected(int connectionId);
    void received(int connectionId, const QByteArray& msg);

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
    ~TcpConnection();

    void close();
    void write(const QByteArray& data);

public slots:
    void socketDisconnect();
    void readerReceived(const QByteArray& data);
    void socketError(QAbstractSocket::SocketError);

private:
    int id;
    QTcpSocket* socket;
    MessageReader* _reader;
    CommTcpServer* _comm;


};


#endif // COMMCHANNELSERVER_H
