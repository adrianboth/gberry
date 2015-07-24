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
 
 #ifndef COMMTCPCLIENT_H
#define COMMTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class MessageReader;


class CommTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit CommTcpClient(int port, QObject *parent = 0);
    ~CommTcpClient();

    void open();
    void close();
    bool isConnected();

    void write(int channelId, const QByteArray msg);

signals:
    void connected();
    void disconnected();
    void received(int channelId, const QByteArray msg);


public slots:
    void connectionError(QAbstractSocket::SocketError socketError);
    void socketDisconnected();

private:
    int _port;
    QTcpSocket* _socket;
    MessageReader* _reader;
};

#endif // COMMTCPCLIENT_H
