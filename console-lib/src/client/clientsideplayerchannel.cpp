#include "clientsideplayerchannel.h"

ClientSidePlayerChannel::ClientSidePlayerChannel(int channelId, PlayerMetadata& metdata, QObject* qparent) :
    PlayerChannel(channelId, NULL, qparent), _playerMeta(metdata)
{
}

ClientSidePlayerChannel::~ClientSidePlayerChannel()
{
}
