#include "playermeta.h"

PlayerMeta::PlayerMeta() : playerId(-1), playerName("UNKNOWN")
{
}

PlayerMeta::PlayerMeta(int pid, QString name) :
    playerId(pid), playerName(name)
{
}

PlayerMeta::PlayerMeta(const QJsonObject& metaJson)
{
    this->playerId = metaJson["id"].toInt();
    this->playerName = metaJson["name"].toString();
}

PlayerMeta::PlayerMeta(const PlayerMeta& other)
{
    this->playerId = other.playerId;
    this->playerName = other.playerName;
}

PlayerMeta& PlayerMeta::operator=(const PlayerMeta& other)
{
    this->playerId = other.playerId;
    this->playerName = other.playerName;
    return *this;
}

QJsonObject PlayerMeta::json()
{
    QJsonObject json;
    json["id"] = playerId;
    json["name"] = playerName;
    return json;
}
