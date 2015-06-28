#include "connection.h"

#include "client/clientsidecontrolchannel.h"

class ConnectionPrivate
{
public:
    explicit ConnectionPrivate(ClientSideControlChannel* controlChannel_) :
        controlChannel(controlChannel_) {}

    ClientSideControlChannel* controlChannel;
};

Connection::Connection(ClientSideControlChannel* controlChannel, QObject *parent) :
    QObject(parent),
    _d(new ConnectionPrivate(controlChannel))
{

    connect(controlChannel, &ClientSideControlChannel::isConnectedChanged,
            this, &Connection::onConnectedChanged);

    connect(controlChannel, &ClientSideControlChannel::isActivatedChanged,
            this, &Connection::onActivatedChanged);
}

Connection::~Connection()
{
}

bool Connection::isConnected() const
{
    return _d->controlChannel->isConnected();
}

bool Connection::isActivated() const
{
    return _d->controlChannel->isActivated();
}

void Connection::onActivatedChanged()
{
    emit isActivatedChanged();
}

void Connection::onConnectedChanged()
{
    emit isConnectedChanged();
}
