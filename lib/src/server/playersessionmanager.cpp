#include "playersessionmanager.h"
#include "server/playersession.h"

PlayerSessionManager::PlayerSessionManager(QObject *parent) :
    QObject(parent)
{
}

PlayerSessionManager::~PlayerSessionManager()
{
}

void PlayerSessionManager::insertSession(PlayerSession& session)
{
    _sessions.insert(session.token(), session);
}

PlayerSession PlayerSessionManager::sessionByToken(QString token)
{
    if (_sessions.contains(token))
    {
        return _sessions[token];
    }
    return InvalidPlayerSession();
}
