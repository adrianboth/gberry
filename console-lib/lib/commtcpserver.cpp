#include "commtcpserver.h"

#include <QTcpServer>
#include <QTcpSocket>

#include "messagereader.h"


TcpConnection::TcpConnection(int id, QTcpSocket* s, CommTcpServer* ccs) :
    id(id), socket(s), _comm(ccs), _reader(NULL)
{
    _reader = new MessageReader(socket);

    connect(socket,     &QTcpSocket::disconnected,
            this,       &TcpConnection::socketDisconnect);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), // error(...) overload, any c++11 way to select specific function
            this,   SLOT(socketError(QAbstractSocket::SocketError)));


    connect(_reader, &MessageReader::received,
            this,    &TcpConnection::readerReceived);
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::close()
{
    _comm = NULL; // no more signal redirection
    socket->close(); // wait to get socketDisconnect()
}

void TcpConnection::socketDisconnect()
{
    if (_comm)
        _comm->closeConnection2(id); // TODO: inform, remove data structure

    delete _reader;
    _reader = NULL;

    socket->deleteLater();
    socket = NULL;

    this->deleteLater(); // TODO: we could disconnect and delete in different places but ...
}

void TcpConnection::readerReceived(const QByteArray &data)
{
    if (!_comm)
        return;

    // ... seems like accepted syntax
    emit _comm->received(id, data);
}

void TcpConnection::socketError(QAbstractSocket::SocketError err)
{
    // TODO: looks like remote connection closed comes also here, ignore as disconnected also attached
    // QAbstractSocket::RemoteHostClosedError (1)
    if (!_comm)
        return;
}

void TcpConnection::write(const QByteArray &data)
{
    if (_reader)
        _reader->write(data);
}


CommTcpServer::CommTcpServer(int port, QObject *parent) :
    QObject(parent), _server(NULL), _port(port)
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
        closeConnection2(id);
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
    TcpConnection* connection = new TcpConnection(id, socket, this);
    _connections[id] = connection;

    emit connected(id);
}


void CommTcpServer::closeConnection2(int connectionId)
{
    if (_connections.contains(connectionId))
    {
        TcpConnection* conn = _connections[connectionId];
        _connections.remove(connectionId);

        // we can arrive here two ways
        // 1) socket was closed and TcpConnection informs to delete resources
        // 2) someone wants to close connection
        //
        // So we need to invoke close() for socket but it might cause TcpConnection to be
        // invoked again -> to clarify situation (break loop) we disconnect

        // TODO
        //disconnect(conn->socket, &QTcpSocket::disconnected,
        //           conn,         &TcpConnection::disconnect);

        //conn->socket->close(); // might have been already closed
        //conn->socket->deleteLater();

        //delete conn;
        emit disconnected(connectionId);
    }
}


void CommTcpServer::write(int connectionId, const QByteArray &msg)
{
    if (_connections.contains(connectionId))
    {
        TcpConnection* conn = _connections[connectionId];
        conn->write(msg);
    }
}
