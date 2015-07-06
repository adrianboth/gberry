#include "connection.h"

#include "client/clientsidecontrolchannel.h"

class ConnectionPrivate
{
public:
    explicit ConnectionPrivate(ClientSideControlChannel* controlChannel_) :
        controlChannel(controlChannel_) {}

    ClientSideControlChannel* controlChannel;
    bool isHeadServerConnected{false};
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

bool Connection::isHeadServerConnected() const
{
    return _d->isHeadServerConnected;
}

void Connection::onActivatedChanged()
{
    emit isActivatedChanged();
}

void Connection::onConnectedChanged()
{
    emit isConnectedChanged();
}

void Connection::onHeadServerConnectedChanged()
{
    // TODO: impl, this should read from somewhere or taken as parameter
}
