#include "commtcpserver.h"

#include <QTcpServer>
#include <QTcpSocket>


TcpConnection::TcpConnection(int id, QTcpSocket* s, CommTcpServer* ccs) :
    id(id), socket(s), _comm(ccs), _blockSize(0)
{
}

// TODO: QSignalMapper, or lambda function
void TcpConnection::disconnect()
{
    if (_comm)
        _comm->closeConnection2(id); // TODO: inform, remove data structure
    socket->deleteLater();
    this->deleteLater(); // TODO: we could disconnect and delete in different places but ...
}

void TcpConnection::read()
{
    if (!_comm)
        return;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_4);

    if (_blockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> _blockSize;
    }

    if (socket->bytesAvailable() < _blockSize)
        return;

    // TODO: what if there is second message ready to be read? I guess we don't other readyRead() signal
    QByteArray data;
    in >> data;
    _blockSize = 0; // reset for next message

    // seems like accepted syntax
    emit _comm->received(id, data);
}

void TcpConnection::error(QAbstractSocket::SocketError err)
{
    // TODO: looks like remote connection closed comes also here, ignore as disconnected also attached
    // QAbstractSocket::RemoteHostClosedError (1)
    if (!_comm)
        return;
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

    foreach (int id, _connections.keys()) {
        TcpConnection* conn = _connections[id];
        conn->_comm = NULL;
        conn->socket->close();
        closeConnection2(id);
    }
    _server->deleteLater();
    _server = NULL;
}


void CommTcpServer::newConnection()
{
    QTcpSocket* socket = _server->nextPendingConnection();

    // TODO: not sure do we need connectionId, and does it mean
    //       anything if just generated (it could have different for mainui vs app)
    TcpConnection* connection = new TcpConnection(_connectionIdCount++, socket, this);
    _connections[connection->id] = connection;

    connect(socket,     &QTcpSocket::disconnected,
            connection, &TcpConnection::disconnect);

    connect(socket,     &QTcpSocket::readyRead,
            connection, &TcpConnection::read);

    connect(socket,    SIGNAL(error(QAbstractSocket::SocketError)),
            connection, SLOT(error(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function


    emit connected(connection->id);

    /*
    // TODO: data parser
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);

    // if we want to keep connection to directional, no disconnectFromHost() ?
    clientConnection->disconnectFromHost();

    // TODO: how to read data, get signal when data is available
    */
}


void CommTcpServer::closeConnection2(int id)
{
    if (_connections.contains(id))
    {
        TcpConnection* conn = _connections[id];
        _connections.remove(id);

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
        emit disconnected(id);
    }
}


void CommTcpServer::write(int connectionId, QByteArray &data)
{
    // TODO
}
