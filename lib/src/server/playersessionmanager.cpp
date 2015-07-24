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
