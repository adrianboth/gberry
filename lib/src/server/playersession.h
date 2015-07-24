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


class PlayerSession : public QObject
{
    Q_OBJECT

public:
    PlayerSession();
    explicit PlayerSession(int playerId, QString playerName, QString token);
    PlayerSession(const PlayerSession& other);
    ~PlayerSession();

    PlayerSession& operator =(const PlayerSession& other);

    int playerId() { return _id; }
    QString playerName() { return _name; }
    QString token() { return _token; }

    bool isValid() { return _token.length() > 0; }

    // TODO: who generates tokens

    // TODO:guest players

signals:

public slots:

private:
    int _id;
    QString _name;
    QString _token;
};


class InvalidPlayerSession : public PlayerSession
{
public:
    InvalidPlayerSession() : PlayerSession(-1, "INVALID", "") {}

};


class GuestPlayerSession : public PlayerSession
{
public:
    GuestPlayerSession(QString guestName, QString token);
};

#endif // PLAYERSESSION_H
