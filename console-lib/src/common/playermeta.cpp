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
