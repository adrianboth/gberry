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
 
#include "testutils/qtgtest.h"
#include "testutils/waiter.h"

#include "server/playersessionmanager.h"
#include "server/consolerestserver.h"
#include "server/websocketserver.h"
#include "client/consolesessionmanager.h"

using namespace GBerryClient;

#define LOG_AREA "RESTAPITests"
#include "log/log.h"

TEST(RESTAPI, UserLoginMeta)
{
    UserLoginMeta guest("FooGuest");
    EXPECT_TRUE(guest.isGuest());
    EXPECT_TRUE(guest.userName() == "FooGuest");
    EXPECT_TRUE(guest.userToken().isEmpty());

    UserLoginMeta player("FooPlayer", "token123");
    EXPECT_FALSE(player.isGuest());
    EXPECT_TRUE(player.userName() == "FooPlayer");
    EXPECT_TRUE(player.userToken() == "token123");
}

TEST(RESTAPI, ConsoleRESTServerAndConsoleSessionManagerIntegration)
{
    PlayerSessionManager serverSessionManager;
    ConsoleRESTServer restServer(serverSessionManager);
    WebsocketServer websocketServer(&serverSessionManager);
    websocketServer.start();

    ConsoleSessionManager clientSessionManager;

    bool clientSessionIsOpen = false;
    QObject::connect(&clientSessionManager, &ConsoleSessionManager::consoleSessionOpened,
                     [&] () { clientSessionIsOpen = true; });

    ASSERT_FALSE(serverSessionManager.isPlayerNameReserved("BarGuest"));

    // --
    ConsoleDevice console("localhost");
    UserLoginMeta loginmeta("BarGuest");
    clientSessionManager.open(console, loginmeta);

    WAIT_CUSTOM_AND_ASSERT(clientSessionIsOpen, 5000, 50);

    ASSERT_TRUE(serverSessionManager.isPlayerNameReserved("BarGuest"));

    // because playerIds are coming from static counter other tests may affect
    // to value -> we need to fetch via debug API
    QMap<QString, PlayerSession> sessionsDebug = serverSessionManager.debug_session();
    ASSERT_EQ(1, sessionsDebug.size());

    PlayerSession session = sessionsDebug.values().at(0);

    ASSERT_TRUE(session.isValid()) << session.playerId() << session.playerName();
    ASSERT_TRUE(session.isGuest());
    ASSERT_TRUE(session.playerName() == "BarGuest");

    ASSERT_TRUE(clientSessionManager.isOpen());

}


TEST(RESTAPI, ConsoleRESTServerAndConsoleSessionManagerIntegration_RegisteredPlayer)
{
    PlayerSessionManager serverSessionManager;
    ConsoleRESTServer restServer(serverSessionManager);
    WebsocketServer websocketServer(&serverSessionManager);
    websocketServer.start();

    ConsoleSessionManager clientSessionManager;

    bool clientSessionIsOpen = false;
    QObject::connect(&clientSessionManager, &ConsoleSessionManager::consoleSessionOpened,
                     [&] () { clientSessionIsOpen = true; });

    ASSERT_FALSE(serverSessionManager.isPlayerNameReserved("FooBar"));

    // --
    ConsoleDevice console("localhost");
    UserLoginMeta loginmeta("FooBar", "token123");
    clientSessionManager.open(console, loginmeta);

    WAIT_CUSTOM_AND_ASSERT(clientSessionIsOpen, 5000, 50);

    ASSERT_TRUE(serverSessionManager.isPlayerNameReserved("FooBar"));
    // we just happen to know how ids are generated, first is 1
    PlayerSession session = serverSessionManager.session(1);
    ASSERT_TRUE(session.isValid());
    ASSERT_FALSE(session.isGuest());
    ASSERT_TRUE(session.playerName() == "FooBar");

    ASSERT_TRUE(clientSessionManager.isOpen());


}
