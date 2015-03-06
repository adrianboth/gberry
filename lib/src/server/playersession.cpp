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
