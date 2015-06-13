#include <gtest/gtest.h>

#include <QObject>

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "server/serverchannelmanager.h"
#include "server/serversideplayerchannel.h"
#include "server/commtcpserver.h"
#include "server/serversidecontrolchannel.h"
#include "server/serversetup.h"
#include "server/channelfactory.h"
#include "client/clientchannelmanager.h"
#include "client/clientsidecontrolchannel.h"
#include "client/commtcpclient.h"
#include "client/clientsetup.h"
#include <client/qmlapplication.h>
#include <client/consoledevice.h>

#include "utils/testchannelfactory.h"

// TEST
#include <restinvocationfactoryimpl.h>
#include <server/consolerestserver.h>


#define LOG_AREA "test_communication_integration"
#include "log/log.h"

TEST(CommunicationIntegration, SetupServerAndClientRootChannelsAndPingBothWays)
{
    // TESTING: basic message flow
    //  - we are setting up objects manually

    CommTcpServer server(7777); // server is comms

    TestChannelFactory factory;
    ServerChannelManager serverChannelManager(&factory);

    int connectionId = -1;
    // we discard connectionId, in future it might have meaning
    QObject::connect(&server, &CommTcpServer::received,
            [&] (int connectionId_, int channelId, const QByteArray msg) {
        connectionId = connectionId_;
        serverChannelManager.processMessageFromSouth(connectionId_, channelId, msg);
    });

    QObject::connect(&serverChannelManager, &ServerChannelManager::outgoingMessageToSouth,
                     [&server] (int connectionId, int channelId, const QByteArray msg) {
        server.write(connectionId, channelId, msg); // connection id count should have started from zero
    });

    // ServerChannelManager is informed if whole connection is closed
    QObject::connect(&server, &CommTcpServer::disconnected,
                     [&] (int connectionId) { serverChannelManager.applicationClosed(connectionId); });

    server.open();

    // --
    CommTcpClient client(7777); // client is any app

    ClientChannelManager clientChannelManager;
    QObject::connect(&client,               &CommTcpClient::received,
                     &clientChannelManager, &ClientChannelManager::processMessage);

    int disconnectedReceived = 0;
    QObject::connect(&client,               &CommTcpClient::disconnected,
                     [&] () { disconnectedReceived++; });

    QObject::connect(&clientChannelManager, &ClientChannelManager::outgoingMessage,
                     &client, &CommTcpClient::write);

    ClientSideControlChannel clientControl;
    clientChannelManager.registerControlChannel(&clientControl);

    int clientPingReceived = 0;
    QObject::connect(&clientControl, &ClientSideControlChannel::pingReceived, [&] () { clientPingReceived++; });

    client.open();
    Waiter::wait([&] () { return client.isConnected(); });
    // TODO: should we check connected() signal or is that done elsewhere

    // connection should have been handshaked

    // -- test
    ASSERT_TRUE(factory.lastControlChannel);
    ServerSideControlChannel* serverControl = factory.lastControlChannel;
    int serverPingReceived = 0;
    QObject::connect(factory.lastControlChannel, &ServerSideControlChannel::pingSouthReceived, [&] () { serverPingReceived++; });
    int channelCloseCalled = 0;
    QObject::connect(factory.lastControlChannel, &ServerSideControlChannel::channelClosed, [&] () { channelCloseCalled++; });
    clientControl.ping();

    Waiter::wait([&] () { return serverPingReceived > 0; });
    EXPECT_EQ(serverPingReceived, 1);

    // answer should come back
    Waiter::wait([&] () { return clientPingReceived > 0; });
    EXPECT_EQ(clientPingReceived, 1);

    //EXPECT_TRUE(clientControl.isOpen());
    EXPECT_TRUE(serverControl->isOpen());

    // other way round: server -> client
    serverControl->pingSouth();

    Waiter::wait([&] () { return clientPingReceived > 1; });
    EXPECT_EQ(clientPingReceived, 2);

    // answer should come back
    Waiter::wait([&] () { return serverPingReceived > 1; });
    EXPECT_EQ(serverPingReceived, 2);

    // -- closing
    client.close(); // whole tcp connection from client to server is closed: i.e. application closed

    WAIT_AND_ASSERT(channelCloseCalled == 1);

    // On client side closing means tearing down all data structures.
    // Now checking that just signal arrives
    EXPECT_EQ(disconnectedReceived, 1);
    // TODO: when client connection goes down, all control should be recreated

    // -- closing
    server.close(); // just to be nice
    TRACE("Tearing down PingBothWays");
}


TEST(CommunicationIntegration, NewPlayerChannelCreatedAndDestroyed)
{
    ServerSetup serverSetup;
    serverSetup.start();

    // --
    ClientSetup clientSetup;
    clientSetup.start();

    Waiter::wait([&] () { return clientSetup.tcpClient.isConnected(); });
    // TODO: should we check connected() signal or is that done elsewhere

    // -- new player (remember server is 'comms')

    PlayerMeta playerMeta(100, "foobar");

    ServerSidePlayerChannel* clientPlayer= serverSetup.channelManager->openPlayerChannel(playerMeta);
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() > 0; }, true);
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 1);

    // Waiting OpenChannelAccepted to arrive
    Waiter::wait([&] () { return clientPlayer->isOpen(); }, true);

    // -- close

    serverSetup.channelManager->closePlayerChannel(clientPlayer->channelId()); // sends closing message, no response
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() < 1; });
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 0);

    // closing happens in setup class desctructors
}


TEST(CommunicationIntegration, WhenClientDisconnectsAllServerSideChannelHandlersClosedAndReconnectedLater)
{
    TestChannelFactory factory;
    ServerSetup serverSetup;
    serverSetup.use(&factory);

    serverSetup.start();

    ClientSetup* clientSetup = new ClientSetup();
    clientSetup->start();

    Waiter::wait([&] () { return clientSetup->tcpClient.isConnected(); });

    PlayerMeta playerMeta(100, "FooPlayer");

    ServerSidePlayerChannel* serverSidePlayer = serverSetup.channelManager->openPlayerChannel(playerMeta);

    // Waiting OpenChannelAccepted to arrive (OPEN_ONGOING -> OPEN)
    Waiter::wait([&] () { return serverSidePlayer->isOpen(); }, true);

    // -- close
    int channelCloseCalled = 0;
    ASSERT_TRUE(factory.lastControlChannel);
    QObject::connect(factory.lastControlChannel, &ServerSideControlChannel::channelClosed,
                     [&] () { channelCloseCalled++;} );

    // when serverside notices disconnect all channels should be closed
    clientSetup->tcpClient.close();
    WAIT_AND_ASSERT(!serverSidePlayer->isOpen());
    ASSERT_TRUE(channelCloseCalled == 1);
    factory.lastControlChannel = NULL;

    // TODO: what happens to client side (typically this doesn't happen but ...)
    ASSERT_EQ(clientSetup->playersManager.numberOfPlayers(), 0);

    // test also tearing down (no crashes)
    delete clientSetup;

    // -- reconnect
    // first root channels handshake with ping messages
    // then playerchannels are opened

    clientSetup = new ClientSetup();
    clientSetup->start();
    Waiter::wait([&] () { return clientSetup->tcpClient.isConnected(); });

    // when server side recognizes connection is back it should reconnect
    WAIT_AND_ASSERT(serverSidePlayer->isOpen());
    ASSERT_EQ(clientSetup->playersManager.numberOfPlayers(), 1);
    ASSERT_TRUE(factory.lastControlChannel != NULL);

    // -- tear down
    // closing happens in setup class desctructors
}

/*
TEST(CommunicationIntegration, AppSendsDataAndPlayerResponds)
{
    // TODO: For some reason we get socket disconnect right away.
    //       If run alone OK, but if with others (even when all tests pass)
    //       we get disconnected() signal. Not sure why.
    //         - I guess having proper test fixture with tear down
    //           in any case (wait sockets closed) would solve situation.
    //
    //Waiter::wait([&] () { return false; });
    Waiter::wait([&] () { return false; }, true);

    ServerSetup serverSetup;
    serverSetup.start();

    ClientSetup* clientSetup = new ClientSetup();
    clientSetup->start();

    int playerInReceived = 0;
    int playerIdReceived = -1;
    QObject::connect(&(clientSetup->playersManager), &PlayersManager::playerIn,
                     [&] (int playerId) { playerInReceived++; playerIdReceived = playerId; });

    int playerMessageReceived = 0;
    QByteArray playerInMessage;
    QObject::connect(&(clientSetup->playersManager), &PlayersManager::playerMessageReceived,
                     [&] (int playerId, QByteArray msg) {
        Q_UNUSED(playerId);
        playerMessageReceived++; playerInMessage = msg;
    });

    Waiter::waitAndAssert([&] () { return clientSetup->tcpClient.isConnected(); });
    Waiter::waitAndAssert([&] () { return serverSetup.connectionManager->activeConnection(); });

    PlayerMeta playerMeta(100, "FooPlayer");
    ServerSidePlayerChannel* serverSidePlayer = serverSetup.channelManager->openPlayerChannel(playerMeta);
    Waiter::waitAndAssert([&] ()
        { return serverSidePlayer->isOpen(); }, true);

    int serverPlayerMessageReceived = 0;
    QByteArray serverPlayerMessage;
    QObject::connect(serverSetup.playerConnectionManager, &PlayerConnectionManager::playerMessageFromSouth,
                     [&] (int channelId, const QByteArray msg) {
        Q_UNUSED(channelId);
        serverPlayerMessageReceived++; serverPlayerMessage = msg;
    });

    // --
    QByteArray serverMsg("serverMsg");
    qDebug("###########################");
    serverSidePlayer->receivePlayerMessageFromNorth(serverMsg);
    Waiter::wait([&] () { return playerMessageReceived > 0; }, true, 5000, 500);
    ASSERT_EQ(playerMessageReceived, 1);
    EXPECT_EQ(playerIdReceived, 100);
    EXPECT_TRUE(QString(playerInMessage) == serverMsg);

    QByteArray responseMsg("responseMsg");
    clientSetup->playersManager.sendPlayerMessage(100, responseMsg);
    Waiter::wait([&] () { return serverPlayerMessageReceived > 0; });
    ASSERT_EQ(serverPlayerMessageReceived, 1);
    EXPECT_TRUE(QString(serverPlayerMessage) == responseMsg);

    // -- close nicely
    clientSetup->tcpClient.close();
    Waiter::wait([&] () { return !serverSidePlayer->isOpen(); });
}
*/

// TODO: test client tcp connection dead -> close all channels -> reconnecting

TEST(CommunicationIntegration, FullPipeFromSouthToNorth)
{
    ServerSetup serverSetup;
    serverSetup.start();

    ClientSetup southClientSetup; // TODO: this is more like southsideclient
    southClientSetup.start();

    // -- wait south side up
    int playerInReceived = 0;
    int playerIdReceived = -1;
    QObject::connect(&(southClientSetup.playersManager), &PlayersManager::playerIn,
                     [&] (int playerId) { playerInReceived++; playerIdReceived = playerId; });

    int playerMessageReceived = 0;
    QByteArray playerInMessage;
    QObject::connect(&(southClientSetup.playersManager), &PlayersManager::playerMessageReceived,
                     [&] (int playerId, QByteArray msg) {
        Q_UNUSED(playerId);
        playerMessageReceived++; playerInMessage = msg;
    });

    Waiter::waitAndAssert([&] () { return southClientSetup.tcpClient.isConnected(); });
    Waiter::waitAndAssert([&] () { return serverSetup.connectionManager->activeConnection(); });

    // -- north side client

    mobile::QmlApplication northApplication;

    bool consoleConnectionIsOpen = false;
    QObject::connect(&northApplication, &mobile::QmlApplication::consoleConnectionOpened,
                     [&] () { consoleConnectionIsOpen = true; });

    bool northSideClientMessageReceived = false;
    QString northSideClientMessage;
    QObject::connect(&northApplication, &mobile::QmlApplication::playerMessageReceived,
                     [&] (QString msg) { northSideClientMessageReceived = true; northSideClientMessage = msg; });

    northApplication.loginGuest("GuestFoo");

    mobile::ConsoleDevice console("localhost");
    northApplication.openConsoleConnection(console);

    WAIT_CUSTOM_AND_ASSERT(consoleConnectionIsOpen, 5000, 50);

    // wait whole communication channel to open (tcp ip communication back and worth)
    WAIT_AND_ASSERT(playerInReceived == 1);

    // channels open -> send message
    northApplication.sendMessage("test message");
    WAIT_AND_ASSERT(playerMessageReceived == 1);
    EXPECT_TRUE(QString(playerInMessage) == "test message");

    // and then other direction
    southClientSetup.playersManager.sendPlayerMessage(playerIdReceived, "all ok");
    WAIT_AND_ASSERT(northSideClientMessageReceived);
    EXPECT_TRUE(northSideClientMessage == "all ok");

    // -- closing
    northApplication.closeConsoleConnection();

    WAIT_AND_ASSERT(southClientSetup.playersManager.numberOfPlayers() == 0);
    qDebug("DONE");
}


TEST(CommunicationIntegration, FullPipeWithThreeNorthClients)
{
    TestChannelFactory testChannelFactory;
    ServerSetup serverSetup;
    serverSetup.use(&testChannelFactory);
    serverSetup.start();

    ClientSetup* southClientSetup = new ClientSetup(); // TODO: this is more like southsideclient
    southClientSetup->start();

    // -- wait south side up
    QMap<QString, int> playerIdByPlayerName;
    QObject::connect(&(southClientSetup->playersManager), &PlayersManager::playerIn,
                     [&] (int playerId) {
        playerIdByPlayerName[southClientSetup->playersManager.playerName(playerId)] = playerId; });


    int playerMessageReceived = 0;
    QByteArray playerInMessage;
    QObject::connect(&(southClientSetup->playersManager), &PlayersManager::playerMessageReceived,
                     [&] (int playerId, QByteArray msg) {
        Q_UNUSED(playerId);
        playerMessageReceived++; playerInMessage = msg;
    });

    Waiter::waitAndAssert([&] () { return southClientSetup->tcpClient.isConnected(); });
    Waiter::waitAndAssert([&] () { return serverSetup.connectionManager->activeConnection(); });

    // -- north side clients

    mobile::QmlApplication northApplication1;

    QString northSideClient1Message;
    QObject::connect(&northApplication1, &mobile::QmlApplication::playerMessageReceived,
                     [&] (QString msg) { northSideClient1Message = msg; });

    mobile::QmlApplication northApplication2;

    QString northSideClient2Message;
    QObject::connect(&northApplication2, &mobile::QmlApplication::playerMessageReceived,
                     [&] (QString msg) { northSideClient2Message = msg; });

    mobile::QmlApplication northApplication3;

    QString northSideClient3Message;
    QObject::connect(&northApplication3, &mobile::QmlApplication::playerMessageReceived,
                     [&] (QString msg) { northSideClient3Message = msg; });


    mobile::ConsoleDevice console("localhost");

    northApplication1.loginGuest("Guest1Foo");
    northApplication1.openConsoleConnection(console);

    northApplication2.loginGuest("Guest2Foo");
    northApplication2.openConsoleConnection(console);

    northApplication3.loginGuest("Guest3Foo");
    northApplication3.openConsoleConnection(console);

    WAIT_CUSTOM_AND_ASSERT(northApplication1.isConsoleConnectionOpen(), 5000, 50);
    WAIT_CUSTOM_AND_ASSERT(northApplication2.isConsoleConnectionOpen(), 5000, 50);
    WAIT_CUSTOM_AND_ASSERT(northApplication3.isConsoleConnectionOpen(), 5000, 50);

    // wait whole communication channel to open (tcp ip communication back and worth)
    WAIT_AND_ASSERT(southClientSetup->playersManager.numberOfPlayers() == 3);
    ASSERT_TRUE(playerIdByPlayerName.size() == 3);

    // channels open -> send messages (mixed directions)
    northApplication1.sendMessage("test1 message");
    northApplication2.sendMessage("test2 message");
    // TODO: problem -> for guest players need to generate unique ids
    // TODO: by name find out who is who
    southClientSetup->playersManager.sendPlayerMessage(playerIdByPlayerName["Guest3Foo"], "all3 ok");

    WAIT_AND_ASSERT(playerMessageReceived == 2);
    WAIT_AND_ASSERT(northSideClient3Message == "all3 ok");
    // and then other direction
    southClientSetup->playersManager.sendPlayerMessage(playerIdByPlayerName["Guest1Foo"], "all1 ok");
    southClientSetup->playersManager.sendPlayerMessage(playerIdByPlayerName["Guest2Foo"], "all2 ok");
    northApplication2.sendMessage("test3 message");

    WAIT_AND_ASSERT(northSideClient1Message == "all1 ok");
    WAIT_AND_ASSERT(northSideClient2Message == "all2 ok");
    WAIT_AND_ASSERT(playerMessageReceived == 3);

// -- close south side application
    delete southClientSetup;

    // wait situation settle
    WAIT_AND_ASSERT(testChannelFactory.controlChannelClosedCount == 1);

// -- new south side client
    ASSERT_TRUE(testChannelFactory.lastControlChannel == nullptr);

    southClientSetup = new ClientSetup();
    southClientSetup->start();

    playerMessageReceived = 0;
    QObject::connect(&(southClientSetup->playersManager), &PlayersManager::playerMessageReceived,
                     [&] (int playerId, QByteArray msg) {
        Q_UNUSED(playerId);
        playerMessageReceived++; playerInMessage = msg;
    });

    // reconnection occurs
    WAIT_AND_ASSERT(testChannelFactory.lastControlChannel != nullptr);
    WAIT_AND_ASSERT(testChannelFactory.lastControlChannel->isOpen());
    WAIT_AND_ASSERT(southClientSetup->playersManager.numberOfPlayers() == 3);
  // --

    // test sending messages
    southClientSetup->playersManager.sendPlayerMessage(playerIdByPlayerName["Guest1Foo"], "ok1");
    southClientSetup->playersManager.sendPlayerMessage(playerIdByPlayerName["Guest2Foo"], "ok2");
    northApplication3.sendMessage("yes3");

    WAIT_AND_ASSERT(northSideClient1Message == "all1 ok");
    WAIT_AND_ASSERT(northSideClient2Message == "all2 ok");
    WAIT_AND_ASSERT(playerMessageReceived == 1)

// -- closing
    northApplication1.closeConsoleConnection();
    northApplication2.closeConsoleConnection();
    northApplication3.closeConsoleConnection();

    WAIT_AND_ASSERT(southClientSetup->playersManager.numberOfPlayers() == 0);
    qDebug("DONE");
}

