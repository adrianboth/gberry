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
 
 #ifndef WEBSOCKETCONNECTION_H
#define WEBSOCKETCONNECTION_H

#include <QObject>

class QWebSocket;


class WebsocketConnection : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketConnection(int playerId, QWebSocket* socket, QObject *parent = 0);
    ~WebsocketConnection();

    void sendMessage(QString message);

signals:
    void messageReceived(int playerId, QString message);
    void disconnected(int playerdId);

private slots:
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);
    void onSocketDisconnected();

private:
    int _playerId;
    QWebSocket* _socket;
};

#endif // WEBSOCKETCONNECTION_H
