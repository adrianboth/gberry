#ifndef COMMTCPCLIENT_H
#define COMMTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class MessageReader;


class CommTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit CommTcpClient(int port, QObject *parent = 0);
    ~CommTcpClient();

    void open();
    void close();
    bool isConnected();

    void write(int channelId, const QByteArray msg);

signals:
    void connected();
    void disconnected();
    void received(int channelId, const QByteArray msg);


public slots:
    void connectionError(QAbstractSocket::SocketError socketError);
    void socketDisconnected();

private:
    int _port;
    QTcpSocket* _socket;
    MessageReader* _reader;
};

#endif // COMMTCPCLIENT_H
