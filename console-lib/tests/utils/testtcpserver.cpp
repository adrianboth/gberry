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
 
 #include "testtcpserver.h"

#include <QException>

// TODO: better exceptin; test this works
// TODO: how to get visibility what went wrong (for GTest printout)
class MyException : public QException
{
public:
    void raise() const { throw *this; }
    MyException *clone() const { return new MyException(*this); }
};


TestTcpServer::TestTcpServer(int port, QObject *parent) : QObject(parent), socket(NULL)
{
    server = new QTcpServer();
    bool listenOk = server->listen(QHostAddress::Any, port);
    if (!listenOk)
        throw new MyException();

    //server->errorString();
}

TestTcpServer::~TestTcpServer()
{
    if (server)
    {
        server->close();
        if (socket)
        {
            socket->close();
            socket = NULL;
        }

        server->deleteLater();
        server = NULL;
    }
}

bool TestTcpServer::waitForConnection(int timeout)
{
    if (!server->waitForNewConnection(timeout))
    {
        return false;
    }

    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected,
            socket, &QTcpSocket::deleteLater);

    return true;
}
