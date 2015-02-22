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
