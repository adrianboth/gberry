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
 
 #include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonObject>
#include <QJsonDocument>

#include "server/serversideplayerchannel.h"
#include "common/playermeta.h"


TEST(ServerSidePlayerChannel, GoToMainMenu)
{
    const int channelId = 111; // just random number
    const PlayerMeta pmeta(100, "foobar");

    ServerSidePlayerChannel channel(channelId, pmeta);

    int mainMenuRequestedSignalCount{0};
    QObject::connect(&channel, &ServerSidePlayerChannel::playerRequestedMainMenu,
                     [&] () { mainMenuRequestedSignalCount++; });

    QJsonObject json;
    json["action"] = "GoToMainMenu";
    QByteArray msg(QJsonDocument(json).toJson());

// -- test
    channel.receivePlayerMessageFromNorth(msg);

    WAIT_AND_ASSERT(mainMenuRequestedSignalCount == 1);

}

