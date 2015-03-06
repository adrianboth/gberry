#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QMap>

#include "server/playersession.h"


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
class WebsocketConnection;
class PlayerSessionManager;

class WebsocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketServer(PlayerSessionManager* sessionManager, quint16 port = 8888, QObject *parent = 0);
    ~WebsocketServer();

    void start();

    void closePlayerConnection(int playerId);
    void sendPlayerMessage(int playerId, QString message);

signals:
    void playerConnectionClosed(int playerId);
    void newPlayerConnection(PlayerSession newSession);
    void playerMessageReceived(int playerId, QString message);

private slots:
    void onNewConnection();
    void onConnectionDisconnected(int playerId);
    void onConnectionMessageReceived(int playerId, QString message);

private:
    quint16 _port;
    PlayerSessionManager* _sessionManager;
    QWebSocketServer *_server;
    QMap<int, WebsocketConnection *> _connectionByPlayerId;
};

#endif // WEBSOCKETSERVER_H

// Logic of memory management
// --------------------------
//
// QWebsockets can't deleted right away because socket operations might be ongoing
//  * Delete only on disconnected() signal
//
// WebsocketConnection can be deleted right away as it acts just wrapper.
//  * If socket closes (disconnect received) we can delete socket with deleteLater().
//    All references to socket are cleared.
//  * If ordering socket to close then connecting socket to deleteLater() to do
//    deletion when socket is ready.
//  * QWebsocket is connected to deleteLater() to delete itself after close
//
