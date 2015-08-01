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
 
#ifndef CONSOLESESSIONMANAGER_H
#define CONSOLESESSIONMANAGER_H

#include <QObject>
#include <QString>

#include "websocketclient.h"
#include "restinvocation.h"

class InvocationFactory;


namespace GBerryClient
{

class ConsoleDevice
{
public:
    ConsoleDevice(const QString& hostAddress) : _hostAddress(hostAddress) {}
    ~ConsoleDevice() {}

    QString host() const { return _hostAddress; }

private:
    QString _hostAddress;
};


// user information for making login to console
class UserLoginMeta {
public:
    UserLoginMeta(const QString& userName) : _userName(userName) {}
    UserLoginMeta(const QString& userName, const QString& userToken) :
        _userName(userName), _userToken(userToken) {}

    QString userName() const { return _userName; }
    QString userToken() const { return _userToken; }
    bool isGuest() const { return !_userToken.isEmpty(); }

private:
    QString _userName;
    QString _userToken;
};


// TODO: as this is not exposed to QML this file should be located in different folder
/**
 * Opens and closes connection to a console.
 *
 * Internal API, not exposed to QML.
 */
class ConsoleSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleSessionManager(QObject *parent = 0);
    ~ConsoleSessionManager();

    void open(const ConsoleDevice& console, const UserLoginMeta& loginMeta);
    void openWebsocket(QString token);
    void close();

    WebsocketClient& websocket() { return _websocket; }

    bool isOpen() const { return _websocket.isConnected(); }

signals:
    void consoleSessionOpenFailed(QString errorMessage);
    void consoleSessionOpened();

public slots:
    void onOpenConsoleSessionFinished(Invocation* invocation);
    void onOpenConsoleSessionError(Invocation* invocation);
    void onWebsocketConnected();

private:
    void initRESTInvocationFactory(ConsoleDevice console);
    QString _hostAddress;
    WebsocketClient _websocket;
    InvocationFactory* _restInvocationFactory;
};

} // eon

#endif // CONSOLESESSIONMANAGER_H
