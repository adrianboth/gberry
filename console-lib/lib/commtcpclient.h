#ifndef COMMTCPCLIENT_H
#define COMMTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>


class CommTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit CommTcpClient(int port, QObject *parent = 0);
    ~CommTcpClient();

    void open();

    bool isConnected();

    // TODO: close outside
signals:
    void connected();
    void disconnected();
    void received(QByteArray msg);


public slots:
    void dataReady();
    void connectionError(QAbstractSocket::SocketError socketError);
    void sockecDisconnected();

private:
    QTcpSocket* _socket;
    int _blockSize;
    int _port;
};

#endif // COMMTCPCLIENT_H
