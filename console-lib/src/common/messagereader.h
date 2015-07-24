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
 
 #ifndef MESSAGEREADER_H
#define MESSAGEREADER_H

#include <QObject>
#include <QByteArray>

class QTcpSocket;


/**
 * MessageReader reads prefixed length messaged from TCP stream.
 */
class MessageReader : public QObject
{
    Q_OBJECT
public:
    explicit MessageReader(QTcpSocket* socket, QObject *parent = 0);
    ~MessageReader();

    void write(int channelId, const QByteArray msg);

signals:
    void received(int channelId, const QByteArray msg);

public slots:
    void socketReadyRead();

private:
    QTcpSocket* _socket;
    quint32 _blockSize;
    quint32 _channelId;
    QByteArray _data;
};

#endif // MESSAGEREADER_H
