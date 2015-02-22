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

    bool isConnected();

    // TODO: close from outside
signals:
    void connected();
    void disconnected();
    void received(const QByteArray& msg);


public slots:
    void connectionError(QAbstractSocket::SocketError socketError);
    void sockecDisconnected();

private:
    int _port;
    QTcpSocket* _socket;
    MessageReader* _reader;
};

#endif // COMMTCPCLIENT_H
