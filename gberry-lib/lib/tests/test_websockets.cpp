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

#include <gmock/gmock.h>
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::StrictMock;
using ::testing::_;

#include "server/websocketserver.h"
#include "client/websocketclient.h"

#include "mocks/mock_playersessionmanager.h"

TEST(WebSockets, ConnectDisconnectWithEchoImpl)
{
    // test case where server accepts all connections and echoes messages

    MockPlayerSessionManager sessionManager;

    EXPECT_CALL(sessionManager, sessionByToken(QString("12345")))
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
    WAIT_CUSTOM_AND_ASSERT(client.isConnected(), 10000, 50);
    client.sendMessage(("Hello web!"));

    WAIT_AND_ASSERT(clientReceivedMessage.length() > 0);

    // -- second message
    clientReceivedMessage.clear();
    client.sendMessage(("FOOBAR message"));

    WAIT_AND_ASSERT(clientReceivedMessage.length() > 0);
}

