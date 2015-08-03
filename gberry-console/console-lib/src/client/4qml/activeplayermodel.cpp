#include "activeplayermodel.h"

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
            emit activePlayerIsGuest();
        }

    } else if (playerId != _d->activePlayerId) {
        int oldId = _d->activePlayerId;
        _d->activePlayerId = playerId;
        emit activePlayerIdChanged();
        emit hasActivePlayerChanged();
        if (oldId < 0) {
            // changed from registered player to guest player
            emit activePlayerIsGuest();
        }
    }
}

void ActivePlayerModel::deactivatePlayer()
{
    if (hasActivePlayer()) {
        int oldId = _d->activePlayerId;
        _d->activePlayerId = NO_ACTIVE_PLAYER;

        emit activePlayerIdChanged();
        emit hasActivePlayerChanged();
        if (oldId < 0) {
            emit activePlayerIsGuest();
        }
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
    return _d->activePlayerId < -1;
}

} // eon
