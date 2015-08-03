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

#include "client/cppapplication.h"


namespace GBerryClient
{

QmlApplication::QmlApplication(IConsoleApplication* consoleApp, QObject *parent) :
    QObject(parent),
    _consoleApp(consoleApp),
    _loggedIn(false)
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

void QmlApplication::openConsoleConnection(const QString& hostName)
{
    UserModel& users = _consoleApp->userModel();
    if (users.currentIsGuest()) {
        _consoleSession.open(ConsoleDevice(hostName),
                            UserLoginMeta(users.currentUserName()));

    } else {
        LoginModel& login = _consoleApp->loginModel();
        _consoleSession.open(ConsoleDevice(hostName),
                            UserLoginMeta(users.currentUserName(),
                                          login.userToken()));
    }
}

void QmlApplication::closeConsoleConnection()
{
    _consoleSession.close();
}

bool QmlApplication::isConsoleConnectionOpen() const
{
    return _consoleSession.isOpen();
}

void QmlApplication::sendMessage(const QString& message)
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

void QmlApplication::onConsoleSessionOpenFailed(const QString& message)
{
    emit consoleConnectionOpenFailed(message);
}

void QmlApplication::onWebsocketMessageReceived(const QString& message)
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
