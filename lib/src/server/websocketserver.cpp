#include "websocketserver.h"

#include <QDebug>
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QUrlQuery>

#include "server/websocketconnection.h"
#include "server/playersessionmanager.h"
#include "server/playersession.h"


WebsocketServer::WebsocketServer(PlayerSessionManager* sessionManager, quint16 port, QObject *parent) :
    QObject(parent),
    _port(port),
    _sessionManager(sessionManager),
    _server(new QWebSocketServer(QStringLiteral("GBerry Server"),
                                 QWebSocketServer::NonSecureMode, this)),
    _connectionByPlayerId()
{
}

WebsocketServer::~WebsocketServer()
{
    _server->close();

    foreach(WebsocketConnection* connection, _connectionByPlayerId.values())
    {
        delete connection;
    }
}

void WebsocketServer::start()
{
    if (_server->listen(QHostAddress::Any, _port)) {
        qDebug() << "Websocket server listening on port" << _port;
        connect(_server, &QWebSocketServer::newConnection,
                this, &WebsocketServer::onNewConnection);

        // TODO: is needed??
        //connect(_server, &QWebSocketServer::closed, this, &WebsocketServer::closed);
    }
}

void WebsocketServer::onNewConnection()
{
    qDebug() << "[WebsocketServer] new connection";
    QWebSocket *pSocket = _server->nextPendingConnection();
    qDebug() << "[WebsocketServer] request url:" << pSocket->requestUrl();

    QUrl url(pSocket->requestUrl());
    qDebug() << "[WebsocketServer] url path:" << url.path();
    if (url.path() != "/open")
    {
        qDebug() << "Unknown action:" << url.path();
        connect(pSocket, &QWebSocket::disconnected,
                pSocket, &QWebSocket::deleteLater);
        pSocket->close();
        return;
    }

    QUrlQuery query(pSocket->requestUrl());
    QString token = query.queryItemValue("token"); // returns "" if no such item
    qDebug() << "[WebsocketServer] token:" << token;

    PlayerSession session = _sessionManager->sessionByToken(token);
    if (!session.isValid())
    {
        qDebug() << "No matching session for token" << token;
        connect(pSocket, &QWebSocket::disconnected,
                pSocket, &QWebSocket::deleteLater);
        pSocket->close();
        return;
    }

    WebsocketConnection* connection = new WebsocketConnection(session.playerId(), pSocket);

    connect(connection, &WebsocketConnection::disconnected,
            this, &WebsocketServer::onConnectionDisconnected);

    connect(connection, &WebsocketConnection::messageReceived,
            this, &WebsocketServer::onConnectionMessageReceived);

    _connectionByPlayerId[session.playerId()] = connection;
    emit newPlayerConnection(session.playerId());
}

// command
void WebsocketServer::closePlayerConnection(int playerId)
{
    if (_connectionByPlayerId.contains(playerId))
    {
        WebsocketConnection* conn = _connectionByPlayerId[playerId];
        _connectionByPlayerId.remove(playerId);
        delete conn; // closes connection
        emit playerConnectionClosed(playerId);
    }
}

// reacting to signal
void WebsocketServer::onConnectionDisconnected(int playerId)
{
    closePlayerConnection(playerId);
}

void WebsocketServer::sendPlayerMessage(int playerId, QString message)
{
    if (_connectionByPlayerId.contains(playerId))
    {
        WebsocketConnection* conn = _connectionByPlayerId[playerId];
        conn->sendMessage(message);
    }
}

void WebsocketServer::onConnectionMessageReceived(int playerId, QString message)
{
    emit onPlayerMessageReceived(playerId, message);
}
