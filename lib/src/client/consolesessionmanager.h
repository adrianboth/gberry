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

#include "consoledevice.h"
#include "websocketclient.h"
#include "restinvocation.h"

class InvocationFactory;


namespace mobile
{

class ConsoleSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleSessionManager(QObject *parent = 0);
    ~ConsoleSessionManager();

    void open(ConsoleDevice console, QString playerName);
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
