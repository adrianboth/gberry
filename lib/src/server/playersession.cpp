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
    QObject(NULL),
    _id(-1),
    _name("EMPTY"),
    _token("")
{
}

PlayerSession::PlayerSession(int playerId, QString playerName, QString token) :
    QObject(NULL), _id(playerId), _name(playerName), _token(token)
{

}

PlayerSession::PlayerSession(const PlayerSession &other) :
    QObject(NULL), _id(other._id), _name(other._name), _token(other._token)
{

}

PlayerSession::~PlayerSession()
{

}

PlayerSession& PlayerSession::operator =(const PlayerSession& other)
{
    this->_id = other._id;
    this->_name = other._name;
    this->_token = other._token;
    return *this;
}

// ----------------

namespace
{
    static int __guestIdCounter = -2;
}

GuestPlayerSession::GuestPlayerSession(QString guestName, QString token) :
    PlayerSession(__guestIdCounter--, guestName, token)
{
}
