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
 
 #include "qmlapplication.h"

namespace mobile
{

QmlApplication::QmlApplication(QObject *parent) : QObject(parent), _loggedIn(false)
{
    connect(&_consoleSession, &ConsoleSessionManager::consoleSessionOpened,
            this, &QmlApplication::onConsoleSessionOpened);

    connect(&_consoleSession, &ConsoleSessionManager::consoleSessionOpenFailed,
            this, &QmlApplication::onConsoleSessionOpenFailed);

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
    _loggedIn = true;
    emit loggedInChanged();
}

void QmlApplication::onConsoleSessionOpenFailed(QString message)
{
    emit consoleConnectionOpenFailed(message);
}

void QmlApplication::onWebsocketMessageReceived(QString message)
{
    emit playerMessageReceived(message);
}

void QmlApplication::onWebsocketClosed()
{
    emit consoleConnectionClosed();
    _loggedIn = false;
    emit loggedInChanged();
}

} // eon
