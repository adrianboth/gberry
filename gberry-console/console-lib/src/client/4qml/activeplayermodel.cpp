#include "activeplayermodel.h"

#define LOG_AREA "ActivePlayerModel"
#include <log/log.h>

namespace GBerryApplication {

static const int NO_ACTIVE_PLAYER{0};

class ActivePlayerModel::Private {
public:
    int activePlayerId{NO_ACTIVE_PLAYER};
};

ActivePlayerModel::ActivePlayerModel() :
    _d(new Private)
{
}

ActivePlayerModel::~ActivePlayerModel()
{
}

void ActivePlayerModel::activatePlayer(int playerId)
{
    // -1 means common development user (mouse & keyboard)
    if (playerId == -1 && _d->activePlayerId != -1) {
        int oldId = _d->activePlayerId;
        _d->activePlayerId = -1;
        emit activePlayerIdChanged();
        emit hasActivePlayerChanged();
        if (oldId > 0) {
            // changed from guest to registered player
            emit activePlayerIsGuestChanged();
        }
        DEBUG("Development user activated");

    // is there a change?
    } else if (playerId != _d->activePlayerId) {
        int oldId = _d->activePlayerId;
        bool oldHasActivePlayer = hasActivePlayer();
        bool oldIsGuest = activePlayerIsGuest();

        _d->activePlayerId = playerId;
        emit activePlayerIdChanged();

        bool newIsGuest = activePlayerIsGuest();
        bool newHasActivePlayer = hasActivePlayer();

        if (oldHasActivePlayer != newHasActivePlayer) {
            emit hasActivePlayerChanged();
        }

        if (oldIsGuest != newIsGuest) {
        //if ((oldId < 0 && playerId > 0) || (oldId > 0 && playerId < 0)) {
            // changed from registered player to guest player or wise versa
            emit activePlayerIsGuestChanged();
        }

        DEBUG("Player activated: pid =" << playerId << ", was pid =" << oldId);
    }
    //emit activePlayerIsGuestChanged();
}

void ActivePlayerModel::deactivatePlayer()
{
    if (hasActivePlayer()) {
        int oldId = _d->activePlayerId;
        bool oldHasActivePlayer = hasActivePlayer();

        _d->activePlayerId = NO_ACTIVE_PLAYER;
        emit activePlayerIdChanged();

        bool newHasActivePlayer = hasActivePlayer();
        if (oldHasActivePlayer != newHasActivePlayer) {
            emit hasActivePlayerChanged();
        }

        if (oldId > NO_ACTIVE_PLAYER) {
            emit activePlayerIsGuestChanged();
        }

        DEBUG("Decativated player: was pid =" << oldId);
    }
}

bool ActivePlayerModel::isActivePlayerDevelopmentUser()
{
    return _d->activePlayerId == -1;
}

bool ActivePlayerModel::hasActivePlayer() const
{
    return _d->activePlayerId != NO_ACTIVE_PLAYER;
}

int ActivePlayerModel::activePlayerId() const
{
    return _d->activePlayerId;
}

bool ActivePlayerModel::activePlayerIsGuest() const
{
    return _d->activePlayerId <= NO_ACTIVE_PLAYER; // having no active player is like guest
}

} // eon
