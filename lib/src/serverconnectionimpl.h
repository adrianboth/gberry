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
