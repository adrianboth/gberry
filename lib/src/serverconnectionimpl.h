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
 
 #ifndef SERVERCONNECTIONIMPL_H
#define SERVERCONNECTIONIMPL_H

#include <QObject>
#include "serverconnection.h"

class InvocationFactory;

namespace GBerry {
class Invocation;
}
using namespace GBerry;


class ServerConnectionImpl : public ServerConnection
{
    Q_OBJECT

public:
    ServerConnectionImpl(InvocationFactory* invocationFactory, QObject *parent = 0);
    ~ServerConnectionImpl();

    // inherited
    virtual bool isConnected();
    virtual void open();
    virtual void close();

private slots:
    void pingReady(Invocation* invocation);
    void pingError(Invocation* invocation);
    void pingTimerTriggered();

private:
    void ping();

    InvocationFactory* _factory; // TODO:network manager required
    bool _connected;
    bool _enabled;
    int _pingInterval_ms = 1000;
};

#endif // SERVERCONNECTIONIMPL_H
