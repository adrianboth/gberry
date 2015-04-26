#include "playersessionmanager.h"
#include "server/playersession.h"

PlayerSessionManager::PlayerSessionManager(QObject *parent) :
    QObject(parent)
{
}

PlayerSessionManager::~PlayerSessionManager()
{
}

bool PlayerSessionManager::isPlayerNameReserved(QString& playerName) const
{
    foreach (auto token, _sessions) {

        if (token.playerName() == playerName)
            return true;
    }
    return false;
}

void PlayerSessionManager::insertSession(PlayerSession& session)
{
    _sessions.insert(session.token(), session);
}

void PlayerSessionManager::removeSession(int playerId)
{
    // TODO: what is key? iterate session
    foreach (auto session, _sessions)
    {
        if (session.playerId() == playerId)
        {
            _sessions.remove(session.token());
            break;
        }
    }
}

PlayerSession PlayerSessionManager::sessionByToken(QString token)
{
    if (_sessions.contains(token))
    {
        return _sessions[token];
    }
    return InvalidPlayerSession();
}
