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
 
 #include "websocketclient.h"

#include <QDebug>

#define LOG_AREA "WebsocketClient"
#include "log/log.h"

WebsocketClient::WebsocketClient(QObject *parent) :
    QObject(parent),
    m_url(QUrl("")),
    _isConnected(false)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebsocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebsocketClient::closed);
}

WebsocketClient::~WebsocketClient()
{
}

void WebsocketClient::open(const QUrl &url)
{
    m_url = url;
    qDebug() << "[WebsocketClient] Connection to" << url;
    m_webSocket.open(QUrl(url));
}

void WebsocketClient::close()
{
    m_webSocket.close();
}

bool WebsocketClient::isConnected() const
{
    return _isConnected;
}

void WebsocketClient::sendMessage(QString message)
{
    m_webSocket.sendTextMessage(message);
}

void WebsocketClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebsocketClient::onTextMessageReceived);
    //m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
    _isConnected = true;
    emit connected();
}

void WebsocketClient::onTextMessageReceived(QString message)
{
    TRACE("Message received:" << message);
    // keep open
    //m_webSocket.close();
    emit messageReceived(message);
}
