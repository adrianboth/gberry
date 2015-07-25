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
 
 #ifndef HEADSERVERCONNECTION_H
#define HEADSERVERCONNECTION_H

#include <QObject>
#include <QScopedPointer>

class InvocationFactory;

namespace GBerry {

class Request;
class HeadServerConnectionPrivate;

/**
 * HeadServerConnection is responsible from keeping track of GBerry server
 * (located in Internet) connection.
 *
 * HeadServerConnection will do regular ping REST calls. Also any other
 * succesfull REST call is counted as valid ping.
 *
 * There is connected/disconnected information available for the status of
 * the connection. The idea is not to require constant online connection
 * to the server but provide information when connection is available.
 *
 */
class HeadServerConnection : public QObject
{
    Q_OBJECT
public:
    explicit HeadServerConnection(InvocationFactory* invFactory, QObject *parent = 0);
    ~HeadServerConnection();

    void open();

    bool isConnected() const;

    void makeRequest(Request* request);

    // -- dynamic properties
    static const char* PING_INTERVAL_INT_PROP;
    static const char* PING_TIMEOUT_INT_PROP;

signals:
    void connected();
    void disconnected();

public slots:

private:
    const QScopedPointer<HeadServerConnectionPrivate> _d;
};

} // eon

#endif // HEADSERVERCONNECTION_H

// via ping
// states: not connected, connected, not connection but ping sent - waiting,

// try to ping periodically
//  - normal wait time between pings
//  - if no connection and a request is made -> new ping
//  - if ping fails -> fail the command also

// some kind of request queue ...

// ?? where to get server address -> some kind of general config from somewhere ...
//    (not in this class, but as generally...)



// ?? who will take ownerships of object

// TODO: later this class would take care of registering console to server
//       but in first stage just pinging is fine
