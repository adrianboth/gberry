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
 
 #include "websocketconnection.h"

#include <QtWebSockets/qwebsocket.h>


WebsocketConnection::WebsocketConnection(int playerId, QWebSocket* socket, QObject *parent) :
    QObject(parent), _playerId(playerId), _socket(socket)
{
    connect(_socket, &QWebSocket::textMessageReceived, this, &WebsocketConnection::onTextMessageReceived);
    connect(_socket, &QWebSocket::binaryMessageReceived, this, &WebsocketConnection::onBinaryMessageReceived);
    connect(_socket, &QWebSocket::disconnected, this, &WebsocketConnection::onSocketDisconnected);
}

WebsocketConnection::~WebsocketConnection()
{
    if (_socket)
    {
        // there might be something ongoing, can't delete socket right away
        disconnect(_socket, &QWebSocket::disconnected, this, &WebsocketConnection::onSocketDisconnected);
        connect(_socket, &QWebSocket::disconnected, _socket, &WebsocketConnection::deleteLater);
        _socket->close();
    }
}

void WebsocketConnection::sendMessage(QString message)
{
    if (_socket)
        _socket->sendTextMessage(message);
}

void WebsocketConnection::onTextMessageReceived(QString message)
{
    emit messageReceived(_playerId, message);
}

void WebsocketConnection::onBinaryMessageReceived(QByteArray message)
{
    qWarning("WebsocketConnection::onBinaryMessageReceived() NOT IMPLEMENTED");
    Q_UNUSED(message);
}

void WebsocketConnection::onSocketDisconnected()
{
    if (_socket)
    {
        _socket->deleteLater();
        _socket = NULL;

        emit disconnected(_playerId);
    }
}
