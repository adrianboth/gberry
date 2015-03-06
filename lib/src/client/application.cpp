#include "application.h"

namespace mobile
{

Application::Application(QObject *parent) : QObject(parent)
{
    connect(&_consoleSession, &ConsoleSessionManager::consoleSessionOpened,
            this, &Application::onConsoleSessionOpened);

    connect(&(_consoleSession.websocket()), &WebsocketClient::messageReceived,
            this, &Application::onWebsocketMessageReceived);

    connect(&(_consoleSession.websocket()), &WebsocketClient::closed,
            this, &Application::onWebsocketClosed);
}

Application::~Application()
{
}

void Application::loginGuest(QString guestName)
{
    // TODO: deeper impl, no all players are guests
    _currentPlayerName = guestName;
}

void Application::openConsoleConnection(ConsoleDevice console)
{
    _consoleSession.open(console, _currentPlayerName);
}

void Application::closeConsoleConnection()
{
    _consoleSession.close();
}

bool Application::isConsoleConnectionOpen() const
{
    return _consoleSession.isOpen();
}

void Application::sendMessage(QString message)
{
    if (_consoleSession.isOpen())
    {
        _consoleSession.websocket().sendMessage(message);
    }
}

void Application::onConsoleSessionOpened()
{
    emit consoleConnectionOpened();
}

void Application::onWebsocketMessageReceived(QString message)
{
    emit playerMessageReceived(message);
}

void Application::onWebsocketClosed()
{
    emit consoleConnectionClosed();
}

} // eon
