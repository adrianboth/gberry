#include "tcpconnection.h"

#include <QTcpSocket>
#include "common/messagereader.h"


TcpConnection::TcpConnection(int connectionId, QTcpSocket* s) :
    id(connectionId), socket(s), _reader(NULL)
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
    socket->close(); // wait to get socketDisconnect()
}

void TcpConnection::socketDisconnect()
{
    emit connectionClosed(id);

    delete _reader;
    _reader = NULL;

    socket->deleteLater();
    socket = NULL;

    this->deleteLater(); // TODO: we could disconnect and delete in different places but ...
}

void TcpConnection::readerReceived(int channelId, const QByteArray data)
{
    emit messageReceived(id, channelId, data);
}

void TcpConnection::socketError(QAbstractSocket::SocketError err)
{
    // TODO: looks like remote connection closed comes also here, ignore as disconnected also attached
    // QAbstractSocket::RemoteHostClosedError (1)
}

void TcpConnection::write(int channelId, const QByteArray data)
{
    if (_reader)
        _reader->write(channelId, data);
}
