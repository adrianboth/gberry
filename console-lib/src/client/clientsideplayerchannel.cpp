#include "clientsideplayerchannel.h"

ClientSidePlayerChannel::ClientSidePlayerChannel(int channelId, QObject* qparent) :
    PlayerChannel(channelId, NULL, qparent)
{
}

ClientSidePlayerChannel::~ClientSidePlayerChannel()
{
}
