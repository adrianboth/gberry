/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "websocketserver.h"

#include <QDebug>
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QUrlQuery>

#include "server/websocketconnection.h"
#include "server/playersessionmanager.h"
#include "server/playersession.h"

#define LOG_AREA "WebsockerServer"
#include "log/log.h"


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
        DEBUG("Listening on 0.0.0.0:" << _port);
        connect(_server, &QWebSocketServer::newConnection,
                this, &WebsocketServer::onNewConnection);

        // TODO: is needed??
        //connect(_server, &QWebSocketServer::closed, this, &WebsocketServer::closed);
    }
}

void WebsocketServer::onNewConnection()
{

    QWebSocket *pSocket = _server->nextPendingConnection();
    DEBUG("New connection: request url =" << pSocket->requestUrl().toString());

    QUrl url(pSocket->requestUrl());
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
    DEBUG("Token:" << token);

    PlayerSession session = _sessionManager->sessionByToken(token);
    if (!session.isValid())
    {
        DEBUG("No matching session for token" << token);
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
    emit newPlayerConnection(session);
}

// command
void WebsocketServer::closePlayerConnection(int playerId)
{
    if (_connectionByPlayerId.contains(playerId))
    {
        WebsocketConnection* conn = _connectionByPlayerId[playerId];
        _connectionByPlayerId.remove(playerId);
        delete conn; // closes connection
        _sessionManager->removeSession(playerId);
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
    emit playerMessageReceived(playerId, message);
}
