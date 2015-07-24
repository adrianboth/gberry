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
 
 #ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QJsonObject>
#include <QScopedPointer>

class ClientSideControlChannel;
class ConnectionPrivate;

class IConnectionCommunication : public QObject
{
    Q_OBJECT
public:
    explicit IConnectionCommunication(QObject *parent = 0) : QObject(parent) {}
    virtual ~IConnectionCommunication() {}

    virtual void sendMessage(const QJsonObject& msg) = 0;

signals:
    void messageReceived(const QJsonObject msg);

};


/**
 * @brief Connection information for 4qml
 *
 * This class provides connection information towards qml.
 *   - Connected: If validated TCP connection to 'comms' exists
 *   - Activated: If connected players communicate with our application
 *
 * During life cycle of an app typically both 'connected' and 'activated'
 * change from 'false' to 'true' in during application startup. The
 * application should wait 'connected=true' before sending any commands
 * to 'comms'.
 *
 * For normal applications if these values change back to false it means
 * that that application should be about to close, or it can close itself.
 *
 * However during development time of GBerry platform it is usefull if
 * applications can recover from 'connection going down' i.e. 'comms' is
 * restarted.
 *
 * HeadServerConnection means whether GBerry platform has a connection to
 * GBerry server in Internet. If not then for example querying downloadable
 * applications and downloading them is not possible.
 */
class Connection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(bool isActivated READ isActivated NOTIFY isActivatedChanged)
    Q_PROPERTY(bool isHeadServerConnected READ isHeadServerConnected NOTIFY isHeadServerConnectedChanged)

public:
    explicit Connection(IConnectionCommunication* communication,
                        ClientSideControlChannel* controlChannel,
                        QObject *parent = 0);
    ~Connection();

    bool isConnected() const;
    bool isActivated() const;
    bool isHeadServerConnected() const;

signals:
    void isConnectedChanged();
    void isActivatedChanged();
    void isHeadServerConnectedChanged();

private:
    const QScopedPointer<ConnectionPrivate> _d;

};

#endif // CONNECTION_H
