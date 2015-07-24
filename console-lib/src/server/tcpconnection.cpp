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
