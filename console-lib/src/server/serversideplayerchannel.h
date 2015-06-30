#ifndef SERVERSIDEPLAYERCHANNEL_H
#define SERVERSIDEPLAYERCHANNEL_H

#include "channel.h"
#include "common/playermeta.h"


class PlayerChannelNorthPartner
{
public:
    explicit PlayerChannelNorthPartner(int channelId) : _channelId(channelId) {}
    virtual ~PlayerChannelNorthPartner() {}

    virtual void playerMessageFromSouth(const QByteArray& msg) = 0;

protected:
    int _channelId;
};


class ServerSidePlayerChannel : public Channel
{
    Q_OBJECT

public:
    enum PlayerChannelState {
        CHANNEL_CLOSED,
        CHANNEL_OPEN_ONGOING,
        CHANNEL_OPEN,
        // TODO: currently seems that restricted open doesn't have meaning, because apps handle that ... but is there good reason to handle that general way?
        CHANNEL_RESTRICTED_OPEN // TODO: e.g. joining to game denied (might be just temporary, game ongoing), but restricted channel open to receive information from app (e.g. when joining possible)

    };

    ServerSidePlayerChannel(int channelId,
                            PlayerMeta metadata,
                            QObject* qparent = 0);
    ~ServerSidePlayerChannel();


    void attachNorthPartner(PlayerChannelNorthPartner* partner);
    void detachNorthPartner();

    virtual void receivePlayerMessageFromNorth(const QByteArray& msg);

    // Channel
    virtual bool isOpen() const;
    virtual void open();
    virtual void close();
    virtual bool receiveMessageFromSouth(const QByteArray& msg);

signals:
    void playerRequestedMainMenu();

private:
    PlayerMeta _playerMeta;
    PlayerChannelState _state;
    PlayerChannelNorthPartner* _northPartner;
};

#endif // SERVERSIDEPLAYERCHANNEL_H
