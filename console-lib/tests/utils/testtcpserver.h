#ifndef TESTTCPSERVER_H
#define TESTTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


class TestTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TestTcpServer(int port, QObject *parent = 0);
    ~TestTcpServer();

    QTcpServer* server;
    QTcpSocket* socket;

    bool waitForConnection(int timeout);
};

#endif // TESTTCPSERVER_H
