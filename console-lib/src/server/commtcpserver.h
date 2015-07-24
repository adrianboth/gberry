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
 
 #ifndef COMMTCPSERVER_H
#define COMMTCPSERVER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <QAbstractSocket>


class QTcpServer;
class QTcpSocket;
class TcpConnection;
class MessageReader;

/**
 * @brief TCP communication server
 *
 * Applications connect via TCP to 'comms'. CommTcpServer is server side
 * (i.e. 'comms' side) server accepting TCP connections.
 *
 * On low level it uses specific protocol on top of TCP connection to
 * exchange data. This protocol has multiple channels on top of single
 * TCP connection. Message exchange has channel id for each channel.
 *
 * **Root channel** is opened between application and comms. Root channel
 * id is 0.
 *
 * **Player channels** are created for each player that is connected to
 * comms.
 *
 * TODO: connectionID. Running number for connections
 */
class CommTcpServer : public QObject
{
    Q_OBJECT

public:
    explicit CommTcpServer(int port, QObject *parent = 0);
    ~CommTcpServer();

    void open();
    void close();

    void closeConnection(int connectionId);
    void write(int connectionId, int channelId, const QByteArray msg);

signals:
    void connected(int connectionId); // TODO: when used
    void disconnected(int connectionId);
    void received(int connectionId, int channelId, const QByteArray msg);

public slots:
    void newConnection();

    // TODO: responsibility to read data in prefix lengths (first decoding)
    //        -> forwards messages to MessageHandler
    // TODO: possible to write data to open channel

private:
    QTcpServer* _server;
    int _port;
    QMap<int, TcpConnection*> _connections;
    int _connectionIdCount;
};

#endif // COMMTCPSERVER_H
