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
 
 #include "testhttpserver.h"

#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"


TestHttpServer::TestHttpServer(uint port, QObject *parent) :
    QObject(parent),
    _requestReceived(false),
    _currentReq(nullptr)
{
    _server = new QHttpServer(this);

    connect(_server, &QHttpServer::newRequest,
            this,   &TestHttpServer::handleRequest);

    _server->listen(QHostAddress::LocalHost, port);

}

TestHttpServer::~TestHttpServer()
{
    _server->close();
}

bool TestHttpServer::isRequestReceived() const
{
    return _requestReceived;
}

void TestHttpServer::resetState()
{
    _requestReceived = false;
    _msg.clear();
}

void TestHttpServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    //QByteArray body = "OK";
    //resp->setHeader("Content-Length", QString::number(body.size()));
    //resp->writeHead(200);
    //resp->end(body);

    connect(req, &QHttpRequest::end,
            this, &TestHttpServer::finishRequest);

    // wait all possible body data to be available
    _currentReq = req;

    // TODO: handle this later
    req->storeBody();

    // TODO: memory management ???
    emit request(req, resp);
    qDebug("### EMITTED request()");
}

void TestHttpServer::finishRequest()
{
    qDebug("### finishRequest()");
    _msg = QString::fromLocal8Bit(_currentReq->body());
    _requestReceived = true;
    _currentReq = nullptr;

}
