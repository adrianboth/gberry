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
#include "testutils/qtgtest.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "server/playersessionmanager.h"
#include "server/playersession.h"
#include "server/websocketserver.h"
#include "server/consolerestserver.h"
#include "client/websocketclient.h"
#include "invocationfactoryimpl.h"
#include "restinvocation.h"

#include "testutils/waiter.h"
#include "mocks/mock_playersessionmanager.h"


TEST(Websockets, OpenConnectionAndTransmitData)
{
    // -- create websocket server
    PlayerSessionManager sessionManager;

    WebsocketServer server(&sessionManager);

    int newPlayerId = -1;
    QObject::connect(&server, &WebsocketServer::newPlayerConnection,
                     [&] (PlayerSession s) { newPlayerId = s.playerId(); } );

    int exitPlayerId = -1;
    QObject::connect(&server, &WebsocketServer::playerConnectionClosed,
                     [&] (int playerId) { exitPlayerId = playerId; } );

    int serverReceivedMsgPlayerId = -1;
    QString serverReceivedMsg;
    QObject::connect(&server, &WebsocketServer::playerMessageReceived,
                     [&] (int playerId, QString msg) {
        serverReceivedMsgPlayerId = playerId;
        serverReceivedMsg = msg;
    } );

    // in real situation we would have REST http server accepting logins
    // but now in this test case we insert session manually
    PlayerSession session(100, "foobar", "abc123");
    sessionManager.insertSession(session);

    server.start();

    // --
    // client: open websocket using given token
    QUrl url(QString("ws://localhost:8888/open?token=abc123"));
    WebsocketClient client;

    QString clientReceivedMsg;
    QObject::connect(&client, &WebsocketClient::messageReceived,
                     [&] (QString msg) { clientReceivedMsg = msg; });

    client.open(url);

    // server: accepts connection and verifies token (if not ok -> close)
    // server: WebsocketServer::newPlayerConnection()

    //Waiter::waitAndAssert([&] () { return newPlayerId == 100; });
    //WAIT_AND_ASSERT(newPlayerId == 100);
    Waiter::wait([&] () { return newPlayerId == 100; });
    ASSERT_TRUE(newPlayerId == 100);

    // let connection to be established also in the client end
    Waiter::waitAndAssert([&] () { return client.isConnected(); });

    // ... data transfer ... both ways
    client.sendMessage("foobarmsg");
    Waiter::wait([&] () { return serverReceivedMsg == "foobarmsg"; } );
    ASSERT_TRUE(serverReceivedMsg == "foobarmsg") << "Got: " << serverReceivedMsg;
    //WAIT_AND_ASSERT(serverReceivedMsg == "foobarmsg");
    EXPECT_EQ(serverReceivedMsgPlayerId, 100);

    server.sendPlayerMessage(100, "abc123msg");
    //Waiter::waitAndAssert([&] () { return clientReceivedMsg == "abc123msg"; } );
    WAIT_AND_ASSERT(clientReceivedMsg == "abc123msg");


    // -- closing
    client.close();
    // server notices and send notification
    //Waiter::waitAndAssert([&] () { return exitPlayerId == 100; });
    WAIT_AND_ASSERT(exitPlayerId == 100);

    // normal closing in desctructors
}


TEST(RESTAPI, OpenGuestSession)
{
    MockPlayerSessionManager mockSessionManager;
    ConsoleRESTServer restServer(mockSessionManager);

    InvocationFactoryImpl factory;
    factory.setProperty("url_prefix", "http://localhost:8050/console/v1");

    // -- GET
    RESTInvocation* inv = factory.newRESTInvocation();
    inv->defineGetOperation("/ping");
    inv->execute();
    Waiter::wait([&] () { return false; }, true );

    // TODO: connect to signal and release inv

    // -- POST
    QJsonObject json;
    json["name"] = "fooplayer";
    QJsonDocument jsondoc(json);

    inv = factory.newRESTInvocation();
    inv->definePostOperation("/ping", jsondoc);
    inv->execute();

    Waiter::wait([&] () { return false; }, true );

   // QObject::connect(inv, &RESTInvocation::finishedOK,
   //                  &okRecorder, &SignalRecorder::signal1_QObjectPointer);

   // QObject::connect(inv, &RESTInvocation::finishedError,
   //                  &errRecorder, &SignalRecorder::signal1_QObjectPointer);
    // REST client
    //  - post login (JSON)

    // server updates to PlayerSessionManager
    // return to client token id
}

// on client side Websocket client is connected to Application (qml supported type)
//  application states
//    - not connected
//    - connecting
//    - connect



// client: make REST call to initialize session
//  -> http server puts player session into PlayerSessionManager
//     returns: token

// TODO: this is part of console-lib responsibility, can't be done on gberry-lib level
// (XXX receives newPlayerConnection)
//      -> get by playerId player information from SessionManager
//      -> creates PlayerChannel
//         - if connection to app then open connection right away

// data) connects playerMessageReceived and PlayerChannel::sendMessage
// data) (serversideplayerchannel has pointer to WebsocketServer


// -- closing
// a) client closes
//    WebsocketServer::playerConnectionDisconnected()
//    (session is left, it can be reused), just change state
//
// b) server side closes
//    WebsocketServer::playerConnectionDisconnected()
//    (session is left, it can be reused), just change state





// other test where Session is created through REST api
