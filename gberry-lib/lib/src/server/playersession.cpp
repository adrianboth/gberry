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
 
 #include "playersession.h"

PlayerSession::PlayerSession() :
    QObject(nullptr),
    _id(-1),
    _name("EMPTY")
{
}

PlayerSession::PlayerSession(int playerId,
                             const QString& playerName,
                             const QString& sessionToken,
                             const QString& userToken) :
    QObject(NULL),
    _id(playerId),
    _name(playerName),
    _sessionToken(sessionToken),
    _userToken(userToken)
{
}

PlayerSession::PlayerSession(const PlayerSession &other) :
    QObject(NULL),
    _id(other._id),
    _name(other._name),
    _sessionToken(other._sessionToken),
    _userToken(other._userToken)
{
}

PlayerSession::~PlayerSession()
{
}

PlayerSession& PlayerSession::operator =(const PlayerSession& other)
{
    this->_id = other._id;
    this->_name = other._name;
    this->_sessionToken = other._sessionToken;
    this->_userToken = other._userToken;
    return *this;
}

// ----------------

namespace
{
    // note that 0 and -1 has been reserved
    //  0 = means not defined
    // -1 = development user (mouse + keyboard directly in app)

    static int __playerIdCounter = 1; // player ids are always positive
    static int __guestIdCounter = -2; // guest ids are always negative
}

GuestPlayerSession::GuestPlayerSession(const QString& guestName, const QString& sessionToken) :
    PlayerSession(__guestIdCounter--, guestName, sessionToken, "")
{
}

SignedInPlayerSession::SignedInPlayerSession(const QString& playerName,
                                             const QString& sessionToken,
                                             const QString& userToken) :
    PlayerSession(__playerIdCounter++, playerName, sessionToken, userToken)
{
}
