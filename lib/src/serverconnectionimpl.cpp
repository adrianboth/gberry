#include "serverconnectionimpl.h"

#include "invocationfactory.h"
#include "restinvocation.h"
#include "systemservices.h"


ServerConnectionImpl::ServerConnectionImpl(InvocationFactory* invocationFactory, QObject *parent) :
    ServerConnection(parent),
    _connected(false),
    _enabled(false)
{
    _factory = invocationFactory;
}

ServerConnectionImpl::~ServerConnectionImpl()
{

}

bool ServerConnectionImpl::isConnected()
{
    return _connected;
}

void ServerConnectionImpl::open()
{
    // there is not specific opening routines,
    // just checking whether server is reachable
    _enabled = true;
    ping();
}

void ServerConnectionImpl::close()
{
    // TODO: stop ping timer
    _enabled = false;
    _connected = false;
}

// TODO: is private impl any good from testing point of view?
void ServerConnectionImpl::ping()
{
    RESTInvocation* invocation = _factory->newRESTInvocation();

    connect(invocation, &RESTInvocation::finishedOK,
            this,       &ServerConnectionImpl::pingReady);

    connect(invocation, &RESTInvocation::finishedError,
            this,       &ServerConnectionImpl::pingError);

    invocation->defineGetOperation("/ping");
    invocation->execute();
}


void ServerConnectionImpl::pingReady(Invocation* invocation)
{
    emit ServerConnection::pingOK();

    if (_enabled && !_connected)
    {
        _connected = true;
        emit ServerConnection::connected();
    }

    invocation->deleteLater();

    if (_enabled)
    {
        SystemServices::instance()->singleshotTimer(_pingInterval_ms, this, SLOT(pingTimerTriggered()));
    }

}

void ServerConnectionImpl::pingError(Invocation* invocation)
{
    emit ServerConnection::pingFailure();

    if (_enabled && _connected)
    {
        _connected = false;
        emit ServerConnection::disconnected();
    }

    invocation->deleteLater();

    if (_enabled)
    {
        SystemServices::instance()->singleshotTimer(_pingInterval_ms, this, SLOT(pingTimerTriggered()));
    }
}


void ServerConnectionImpl::pingTimerTriggered()
{
    // close might have occurred while timer was wired
    if (_enabled)
        ping();
}
