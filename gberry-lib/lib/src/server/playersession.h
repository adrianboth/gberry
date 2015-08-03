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
 
#ifndef PLAYERSESSION_H
#define PLAYERSESSION_H

#include <QObject>
#include <QString>

// TODO: why qobject needed??
class PlayerSession : public QObject
{
    Q_OBJECT

public:
    PlayerSession();
    PlayerSession(int playerId,
                  const QString& playerName,
                  const QString& sessionToken,
                  const QString& userToken);
    PlayerSession(const PlayerSession& other);
    virtual ~PlayerSession();

    PlayerSession& operator =(const PlayerSession& other);

    int playerId() const { return _id; }
    QString playerName() const { return _name; }
    QString sessionToken() const { return _sessionToken; }
    QString userToken() const { return _userToken; }
    bool isGuest() { return _userToken.isEmpty(); }
    bool isValid() const { return !_sessionToken.isEmpty(); }

protected:
    int _id;
    QString _name;
    QString _sessionToken;
    QString _userToken;
};


class InvalidPlayerSession : public PlayerSession
{
public:
    // id=-1 reserved for invalidsessions
    InvalidPlayerSession() : PlayerSession(-1, "INVALID", "", "") {}

};


class GuestPlayerSession : public PlayerSession
{
public:
    GuestPlayerSession(const QString& guestName, const QString& sessionToken);
};


class SignedInPlayerSession : public PlayerSession
{
public:
    SignedInPlayerSession(const QString& playerNme, const QString& sessionToken, const QString& userToken);
};

#endif // PLAYERSESSION_H
