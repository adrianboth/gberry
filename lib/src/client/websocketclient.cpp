#include "websocketclient.h"

#include <QDebug>

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

bool WebsocketClient::isConnected()
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
    qDebug() << "Message received:" << message;
    // keep open
    //m_webSocket.close();
    emit messageReceived(message);
}
