#include "connection.h"

#include <QJsonObject>
#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

class ConnectionPrivate
{
public:
    explicit ConnectionPrivate(
            IConnectionCommunication* communication_,
            ClientSideControlChannel* controlChannel_,
            Connection* q_) :
        communication(communication_),
        controlChannel(controlChannel_),
        q(q_) {
        QObject::connect(communication, &IConnectionCommunication::messageReceived,
                        [this] (const QJsonObject& msg) { this->onMessageReceived(msg); });

        QObject::connect(controlChannel, &ClientSideControlChannel::isConnectedChanged,
                        [&] () { this->onConnectedChanged(); });

       QObject::connect(controlChannel, &ClientSideControlChannel::isActivatedChanged,
                        [&] () { this->onActivatedChanged(); });
    }

    IConnectionCommunication* communication;
    ClientSideControlChannel* controlChannel;
    Connection* q;
    bool isHeadServerConnected{false};

    void onActivatedChanged()
    {
        // now we can query headserver connection status
        QJsonObject cmd;
        cmd["command"] = "HeadServerStatus";
        controlChannel->sendMessage(QJsonDocument(cmd).toJson());
        emit q->isActivatedChanged();
    }

    void onConnectedChanged()
    {
        emit q->isConnectedChanged();
    }

    void onHeadServerConnectedChanged(bool connected)
    {
        isHeadServerConnected = connected;
        emit q->isHeadServerConnectedChanged();
    }

    void onMessageReceived(const QJsonObject& json) {
        bool currentConnected = json["status"].toString() == "connected" ? true : false;
        // TODO: needed at all?
        //bool oldConnected = "connected" ? true : false;
        if (isHeadServerConnected != currentConnected) {
            onHeadServerConnectedChanged(currentConnected);
        }
    }
};

Connection::Connection(IConnectionCommunication* communication, ClientSideControlChannel* controlChannel, QObject *parent) :
    QObject(parent),
    _d(new ConnectionPrivate(communication, controlChannel, this))
{
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
