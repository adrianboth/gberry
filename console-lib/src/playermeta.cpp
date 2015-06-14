#include "playermeta.h"

PlayerMeta::PlayerMeta() : _pid(-1), _name("UNKNOWN")
{
}

PlayerMeta::PlayerMeta(int pid, QString name) :
    _pid(pid), _name(name)
{
}

PlayerMeta::PlayerMeta(const QJsonObject& metaJson)
{
    this->_pid = metaJson["id"].toInt();
    this->_name = metaJson["name"].toString();
}

PlayerMeta::PlayerMeta(const PlayerMeta& other)
{
    this->_pid = other._pid;
    this->_name = other._name;
}

PlayerMeta& PlayerMeta::operator=(const PlayerMeta& other)
{
    this->_pid = other._pid;
    this->_name = other._name;
    return *this;
}

QJsonObject PlayerMeta::json()
{
    QJsonObject json;
    json["id"] = _pid;
    json["name"] = _name;
    return json;
}

int PlayerMeta::playerId() const
{
    return _pid;
}

const QString& PlayerMeta::playerName() const
{
    return _name;
}
