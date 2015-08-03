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
    virtual bool isOpen() const override;
    virtual void open() override;
    virtual void close() override;
    //virtual bool receiveMessageFromSouth(const QByteArray& msg) override;
    virtual bool processJsonMessage(const QJsonObject& json) override;

signals:
    void playerRequestedMainMenu();

private:
    PlayerMeta _playerMeta;
    PlayerChannelState _state;
    PlayerChannelNorthPartner* _northPartner;
};

#endif // SERVERSIDEPLAYERCHANNEL_H
