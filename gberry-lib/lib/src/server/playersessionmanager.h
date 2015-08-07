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
 
 #ifndef PLAYERSESSIONMANAGER_H
#define PLAYERSESSIONMANAGER_H

#include <QObject>
#include <QMap>


class PlayerSession;

// TODO: needed to have QObject
class PlayerSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerSessionManager(QObject *parent = 0);
    ~PlayerSessionManager();

    virtual bool isPlayerNameReserved(const QString& playerName) const;
    virtual void insertSession(const PlayerSession& session);
    virtual void removeSession(int playerId);
    virtual PlayerSession sessionByToken(const QString& sessionToken) const;
    virtual PlayerSession session(int playerId) const;

    QMap<QString, PlayerSession>& debug_session() { return _sessions; }

signals:

public slots:

private:
    QMap<QString, PlayerSession> _sessions;
};

#endif // PLAYERSESSIONMANAGER_H
