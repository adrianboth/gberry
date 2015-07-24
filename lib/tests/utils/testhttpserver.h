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
 
 #ifndef TESTHTTPSERVER_H
#define TESTHTTPSERVER_H

#include <QObject>

#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"


class TestHttpServer : public QObject
{
    Q_OBJECT

public:
    explicit TestHttpServer(uint port = 8080, QObject *parent = 0);
    ~TestHttpServer();

    bool isRequestReceived() const;
    QString lastMessageString() const { return _msg; }

    void resetState();

signals:
    // new request arrived
    void request(QHttpRequest *req, QHttpResponse *resp);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
    void finishRequest();

private:
    uint _port;
    QString _path;
    QString _msg;
    bool _requestReceived;
    QHttpRequest* _currentReq;
    QHttpServer* _server;
};

#endif // TESTHTTPSERVER_H
