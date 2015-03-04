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
