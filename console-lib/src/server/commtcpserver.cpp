#include "commtcpserver.h"

#include <QTcpServer>
#include <QTcpSocket>

#include "tcpconnection.h"


CommTcpServer::CommTcpServer(int port, QObject *parent) :
    QObject(parent), _server(NULL), _port(port), _connectionIdCount(0)
{

}

CommTcpServer::~CommTcpServer()
{
    if (_server)
        close();
}

void CommTcpServer::open()
{
    // if already open do not reopen()
    if (_server)
        return;

    _server = new QTcpServer(this);
    if (!_server->listen(QHostAddress::Any, _port)) {
        qCritical() << "Failed to start listening on port " << _port << ": " << _server->errorString();
        close();
        return;
    }

    qDebug() << "[CommTcpServer] Listening on 0.0.0.0:" << _port;
    connect(_server, &QTcpServer::newConnection,
            this,    &CommTcpServer::newConnection);
}

void CommTcpServer::close()
{
    if (_server)
        _server->close();

    disconnect(_server, &QTcpServer::newConnection,
               this,    &CommTcpServer::newConnection);

    foreach (int id, _connections.keys())
    {
        TcpConnection* conn = _connections[id];
        closeConnection(id);
        conn->close();
    }
    _server->deleteLater();
    _server = NULL;
}


void CommTcpServer::newConnection()
{
    QTcpSocket* socket = _server->nextPendingConnection();

    // TODO: not sure do we need connectionId, and does it mean
    //       anything if just generated (it could have different for mainui vs app)
    int id = _connectionIdCount++;
    TcpConnection* connection = new TcpConnection(id, socket);

    connect(connection, &TcpConnection::connectionClosed,
            this, &CommTcpServer::closeConnection);

    // forward signal
    connect(connection, SIGNAL(messageReceived(int, int, const QByteArray)),
            this,       SIGNAL(received(int, int, const QByteArray)));

    _connections[id] = connection;

    emit connected(id);
}


void CommTcpServer::closeConnection(int connectionId)
{
    if (_connections.contains(connectionId))
    {
        // TcpConnection will delete itself later when socket dies finally
        _connections.remove(connectionId);
        emit disconnected(connectionId);
    }
}


void CommTcpServer::write(int connectionId, int channelId, const QByteArray msg)
{
    if (_connections.contains(connectionId))
    {
        TcpConnection* conn = _connections[connectionId];
        conn->write(channelId, msg);
    }
}
