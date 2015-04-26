#include "qmlapplication.h"

namespace mobile
{

QmlApplication::QmlApplication(QObject *parent) : QObject(parent)
{
    connect(&_consoleSession, &ConsoleSessionManager::consoleSessionOpened,
            this, &QmlApplication::onConsoleSessionOpened);

    connect(&(_consoleSession.websocket()), &WebsocketClient::messageReceived,
            this, &QmlApplication::onWebsocketMessageReceived);

    connect(&(_consoleSession.websocket()), &WebsocketClient::closed,
            this, &QmlApplication::onWebsocketClosed);
}

QmlApplication::~QmlApplication()
{
}

void QmlApplication::loginGuest(QString guestName)
{
    // TODO: deeper impl, no all players are guests
    _currentPlayerName = guestName;
}

void QmlApplication::openConsoleConnection(ConsoleDevice console)
{
    _consoleSession.open(console, _currentPlayerName);
}

void QmlApplication::openConsoleConnection(QString hostName)
{
    // TODO: temporary until we know better
    // TODO: does console device has proper copy constructors

    _consoleSession.open(ConsoleDevice(hostName), _currentPlayerName);
}

void QmlApplication::closeConsoleConnection()
{
    _consoleSession.close();
}

bool QmlApplication::isConsoleConnectionOpen() const
{
    return _consoleSession.isOpen();
}

void QmlApplication::sendMessage(QString message)
{
    if (_consoleSession.isOpen())
    {
        _consoleSession.websocket().sendMessage(message);
    }
}

void QmlApplication::onConsoleSessionOpened()
{
    emit consoleConnectionOpened();
}

void QmlApplication::onWebsocketMessageReceived(QString message)
{
    emit playerMessageReceived(message);
}

void QmlApplication::onWebsocketClosed()
{
    emit consoleConnectionClosed();
}

} // eon
