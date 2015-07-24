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
 
 #include "commtcpclient.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

#include "common/messagereader.h"


CommTcpClient::CommTcpClient(int port, QObject *parent) :
    QObject(parent), _port(port), _socket(NULL), _reader(NULL)
{

}

CommTcpClient::~CommTcpClient()
{
    if (_socket)
        close();
}

void CommTcpClient::socketDisconnected()
{
    emit disconnected();
    // someone closed to socket -> reconnect
    // TODO: do we need timeout
    // TODO: if error, we don't get disconnect -> no retry after first error
    open();
}

void CommTcpClient::open()
{
    if (isConnected())
        close();

    _socket = new QTcpSocket();
    _reader = new MessageReader(_socket);
    // forward signal
    connect(_reader, SIGNAL(received(int, const QByteArray)),
            this,    SIGNAL(received(int, const QByteArray)));

    connect(_socket, &QTcpSocket::disconnected,
            this,    &CommTcpClient::socketDisconnected);

    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function

    _socket->connectToHost(QHostAddress::LocalHost, _port);
}

void CommTcpClient::close()
{
    // guard against double closing

    if (_reader)
    {
        delete _reader;
        _reader = NULL;
    }

    if (_socket)
    {
        disconnect(_socket, &QTcpSocket::disconnected,
                   this,    &CommTcpClient::socketDisconnected);

        disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function?

        connect(_socket, &QTcpSocket::disconnected,
                _socket, &QTcpSocket::deleteLater);

        _socket->close();
        _socket = NULL;

        emit disconnected();
    }
}

bool CommTcpClient::isConnected()
{
    return _socket && _socket->state() == QAbstractSocket::ConnectedState;
}


void CommTcpClient::connectionError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        // TODO: action

        break;
    case QAbstractSocket::ConnectionRefusedError:
        // TODO: action

        break;
    default:
        // TODO: action .arg(tcpSocket->errorString()));
        break;
    }
}

void CommTcpClient::write(int channelId, const QByteArray msg)
{
    if (_reader)
        _reader->write(channelId, msg);
}
