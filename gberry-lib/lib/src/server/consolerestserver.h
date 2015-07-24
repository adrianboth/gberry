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
 
 #ifndef CONSOLERESTSERVER_H
#define CONSOLERESTSERVER_H

#include <QObject>
#include "server/playersessionmanager.h"

class QHttpServer;
class QHttpRequest;
class QHttpResponse;

class ConsoleRESTServer : public QObject
{
    Q_OBJECT

public:
    ConsoleRESTServer(PlayerSessionManager& sessionManager);
    ~ConsoleRESTServer();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    // expects all body data to be available
    void handlePostRequest(QHttpRequest *req, QHttpResponse *resp);

    QHttpServer * _server;
    PlayerSessionManager& _sessionManager;
    int _tokenCounter;
};

#endif // CONSOLERESTSERVER_H
