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
 
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

#include "client/consolesessionmanager.h"

namespace GBerryClient
{

// fwds
class CppApplication;

/**
 * @brief Application abstraction for QML side
 *
 * Exposes properties and function to root QML context.
 */
class QmlApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)

public:
    explicit QmlApplication(CppApplication* cppApp, QObject *parent = 0);
    ~QmlApplication();

    Q_INVOKABLE void openConsoleConnection(const QString& hostName);
    Q_INVOKABLE void closeConsoleConnection();
    Q_INVOKABLE bool isConsoleConnectionOpen() const;

    Q_INVOKABLE void sendMessage(const QString& message);

    bool loggedIn() const { return _loggedIn; }

signals:
    void consoleConnectionOpened();
    void consoleConnectionClosed();
    void consoleConnectionOpenFailed(QString errorMsg);
    void playerMessageReceived(QString message);

    void loggedInChanged();

public slots:
    void onConsoleSessionOpened();
    void onConsoleSessionOpenFailed(const QString& message);
    void onWebsocketMessageReceived(const QString& message);
    void onWebsocketClosed();

private:
    CppApplication* _cppApp;
    ConsoleSessionManager _consoleSession;
    bool _loggedIn; // using separate var, as isConsoleConnectionOpen() is about websocket
};

} // eon

#endif // APPLICATION_H
