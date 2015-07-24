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
 
 #ifndef PLAYERMETA_H
#define PLAYERMETA_H

#include <QJsonObject>

// TODO: make this qml compatible
class PlayerMeta
{
public:
    PlayerMeta();
    PlayerMeta(int pid, QString name);
    PlayerMeta(const QJsonObject& metaJson);
    PlayerMeta(const PlayerMeta& other);

    PlayerMeta& operator=(const PlayerMeta& other);

    QJsonObject json();

    int playerId() const;
    const QString& playerName() const;

private:
    int _pid;
    QString _name;
};


#endif // PLAYERMETA_H
