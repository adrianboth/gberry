#include "commchannelserver.h"

#include <QTcpServer>


CommChannelServer::CommChannelServer(int port, QObject *parent) :
    QObject(parent), _server(NULL), _port(port)
{

}

CommChannelServer::~CommChannelServer()
{
    if (_server)
        close();
}

void CommChannelServer::open()
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
            this,    &CommChannelServer::newConnection);
}

void CommChannelServer::close()
{
    if (_server)
        _server->close();

    _server->deleteLater();
    _server = NULL;
}


void CommChannelServer::newConnection()
{
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
