#include "serverconnection.h"

// TODO: private impl

class ServerConnectionImpl
{

public:
    RESTInvocationFactory _factory; // TODO:network manager required
};

timerUpdate()
{
    // TODO: check have got ping -> if not fail -> abort query (do we pingXXX or when we know we can delete object?)
    ping();
}

// TODO: is private impl any good from testing point of view?
void ServerConnectionImpl::ping()
{
    RESTInvocation* invocation = _factory.newInvication();

    connect(invocation, &RESTInvocation::finishedOK,
            this,       &ServerConnectionImpl::pingReady);

    connect(invocation, &RESTInvocation::finishedError,
            this,       &ServerConnectionImpl::pingError);

    invocation->get("/ping");
}


void ServerConnectionImpl::pingReady()
{
    // TODO: record ping ready
    invocation->deleteLater();
}

void ServerConnectionImpl::pingError()
{
    // TODO: record ping failed (e.g. connection failed)
}

// TODO: who can share NetworkAccessManager, what specific
// TODO: should we get ref for factory?
ServerConnection::ServerConnection(RESTInvocationFactory invocationFactory, QObject *parent) : QObject(parent)
{

}

ServerConnection::~ServerConnection()
{

}
