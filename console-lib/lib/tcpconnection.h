#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QByteArray>
#include <QAbstractSocket>

class QTcpSocket;
class MessageReader;


class TcpConnection : public QObject
{
    Q_OBJECT

public:
    TcpConnection(int connectionId, QTcpSocket* s);
    ~TcpConnection();

    void close();
    void write(int channelId, const QByteArray& data);

signals:
    void connectionClosed(int connectionId);
    void messageReceived(int connectionId, int channelId, const QByteArray& msg);

public slots:
    void socketDisconnect();
    void socketError(QAbstractSocket::SocketError);
    void readerReceived(int channelId, const QByteArray& data);

private:
    int id;
    QTcpSocket* socket;
    MessageReader* _reader;

};

#endif // TCPCONNECTION_H
