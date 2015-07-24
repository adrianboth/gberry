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
 
 #ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QByteArray>
#include <QAbstractSocket>

class QTcpSocket;
class MessageReader;


class TcpConnection : public QObject
{
    Q_OBJECT

public:
    TcpConnection(int connectionId, QTcpSocket* s);
    ~TcpConnection();

    void close();
    void write(int channelId, const QByteArray data);

signals:
    void connectionClosed(int connectionId);
    void messageReceived(int connectionId, int channelId, const QByteArray msg);

public slots:
    void socketDisconnect();
    void socketError(QAbstractSocket::SocketError);
    void readerReceived(int channelId, const QByteArray data);

private:
    int id;
    QTcpSocket* socket;
    MessageReader* _reader;

};

#endif // TCPCONNECTION_H
