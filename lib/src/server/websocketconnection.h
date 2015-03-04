#ifndef WEBSOCKETCONNECTION_H
#define WEBSOCKETCONNECTION_H

#include <QObject>

class QWebSocket;


class WebsocketConnection : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketConnection(int playerId, QWebSocket* socket, QObject *parent = 0);
    ~WebsocketConnection();

    void sendMessage(QString message);

signals:
    void messageReceived(int playerId, QString message);
    void disconnected(int playerdId);

private slots:
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);
    void onSocketDisconnected();

private:
    int _playerId;
    QWebSocket* _socket;
};

#endif // WEBSOCKETCONNECTION_H
