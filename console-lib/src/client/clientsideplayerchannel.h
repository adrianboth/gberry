#ifndef CLIENTSIDEPLAYERCHANNEL_H
#define CLIENTSIDEPLAYERCHANNEL_H

#include <QObject>
#include <QByteArray>

#include "common/playermeta.h"

class ClientSideChannelPartner;
class ClientSidePlayerChannelPartner;

class ClientSidePlayerChannel : public QObject
{
    Q_OBJECT

public:
    ClientSidePlayerChannel(int channelId, PlayerMeta& metadata);
    ~ClientSidePlayerChannel();

    void attachChannelPartner(ClientSideChannelPartner* partner);
    void detachChannelPartner();

    void attachPlayerChannelPartner(ClientSidePlayerChannelPartner* partner);
    void detachPlayerChannelPartner();

    int channelId();

    void closeChannel();

    // called when new message from server
    bool receiveMessage(const QByteArray& msg);

    // called when message to a player is wanted to be sent
    void sendPlayerMessage(const QByteArray& msg);

protected:
    int _id;

private:
    ClientSideChannelPartner* _channelPartner;
    ClientSidePlayerChannelPartner* _playerPartner;
    PlayerMeta _playerMeta;
};

#endif // CLIENTSIDEPLAYERCHANNEL_H
