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
 
 #include <gtest/gtest.h>
#include <gmock/gmock.h>
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::StrictMock;
using ::testing::_;

#include "server/websocketserver.h"
#include "client/websocketclient.h"

#include "mocks/mock_playersessionmanager.h"
#include "testutils/waiter.h"


TEST(WebSockets, ConnectDisconnectWithEchoImpl)
{
    // test case where server accepts all connections and echoes messages

    // TODO: mock to return valid sessions
    MockPlayerSessionManager sessionManager; // just to get ref

    EXPECT_CALL(sessionManager, sessionByToken(_))
              .Times(1)
              .WillOnce( Return(GuestPlayerSession("Guest1", "abc123")));

    WebsocketServer server(&sessionManager, 8888);
    QObject::connect(&server, &WebsocketServer::playerMessageReceived,
                     [&] (int playerId, QString msg) { server.sendPlayerMessage(playerId, msg);} );

    server.start();

    // -- client side
    QUrl url(QString("ws://localhost:8888/open?token=12345"));
    WebsocketClient client;

    QString clientReceivedMessage;
    QObject::connect(&client, &WebsocketClient::messageReceived,
                     [&] (QString message) { clientReceivedMessage = message; });

    client.open(url);
    Waiter::waitAndAssert([&] () { return client.isConnected(); });
    client.sendMessage(("Hello web!"));

    Waiter::waitAndAssert([&] () { return clientReceivedMessage.length() > 0; });

    // -- second message
    clientReceivedMessage.clear();
    client.sendMessage(("FOOBAR message"));

    Waiter::waitAndAssert([&] () { return clientReceivedMessage.length() > 0; });
}

