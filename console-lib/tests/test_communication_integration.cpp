#include <gtest/gtest.h>

#include <QObject>

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "channelmanager.h"
#include "server/serversideplayerchannel.h"
#include "server/commtcpserver.h"
#include "server/serversidecontrolchannel.h"
#include "server/serversetup.h"
#include "client/clientsidecontrolchannel.h"
#include "client/commtcpclient.h"
#include "client/clientsetup.h"
#include <client/application.h>
#include <client/consoledevice.h>

// TEST
#include <restinvocationfactoryimpl.h>
#include <server/consolerestserver.h>


TEST(CommunicationIntegration, SetupServerAndClientRootChannelsAndPingBothWays)
{
    // TESTING: basic message flow
    //  - we are setting up objects manually

    CommTcpServer server(7777); // server is comms

    ServerSideChannelManager serverChannelManager;
    // we discard connectionId, in future it might have meaning
    QObject::connect(&server, &CommTcpServer::received,
            [&] (int connectionId, int channelId, const QByteArray msg) {
        Q_UNUSED(connectionId);
        serverChannelManager.processMessage(channelId, msg);
    });

    QObject::connect(&serverChannelManager, &ServerSideChannelManager::outgoingMessage,
                     [&server] (int channelId, const QByteArray msg) {
        server.write(0, channelId, msg); // connection id count should have started from zero
    });

    ServerSideControlChannel serverControl;
    serverChannelManager.registerChannel(&serverControl);

    int serverPingReceived = 0;
    QObject::connect(&serverControl, &ServerSideControlChannel::pingReceived, [&] () { serverPingReceived++; });

    // ServerSideChannelManager is informed if whole connection is closed
    QObject::connect(&server, &CommTcpServer::disconnected,
                     [&] () { serverChannelManager.applicationClosed(); });

    server.open();

    // --
    CommTcpClient client(7777); // client is any app

    ChannelManager clientChannelManager;
    QObject::connect(&client,               &CommTcpClient::received,
                     &clientChannelManager, &ChannelManager::processMessage);

    int disconnectedReceived = 0;
    QObject::connect(&client,               &CommTcpClient::disconnected,
                     [&] () { disconnectedReceived++; });

    QObject::connect(&clientChannelManager, &ClientSideChannelManager::outgoingMessage,
                     &client, &CommTcpClient::write);

    ClientSideControlChannel clientControl;
    clientChannelManager.registerChannel(&clientControl);

    int clientPingReceived = 0;
    QObject::connect(&clientControl, &ClientSideControlChannel::pingReceived, [&] () { clientPingReceived++; });

    client.open();
    Waiter::wait([&] () { return client.isConnected(); });
    // TODO: should we check connected() signal or is that done elsewhere

    // -- test

    clientControl.ping();
    Waiter::wait([&] () { return serverPingReceived > 0; });
    EXPECT_EQ(serverPingReceived, 1);

    // answer should come back
    Waiter::wait([&] () { return clientPingReceived > 0; });
    EXPECT_EQ(clientPingReceived, 1);

    EXPECT_TRUE(clientControl.isOpen());
    EXPECT_TRUE(serverControl.isOpen());

    // other way round: server -> client
    serverControl.ping();

    Waiter::wait([&] () { return clientPingReceived > 1; });
    EXPECT_EQ(clientPingReceived, 2);

    // answer should come back
    Waiter::wait([&] () { return serverPingReceived > 1; });
    EXPECT_EQ(serverPingReceived, 2);

    // pings have put root channels to open state (at least not yet any handshaking
    EXPECT_EQ(serverControl.state(), Channel::CHANNEL_OPEN);
    EXPECT_EQ(clientControl.state(), Channel::CHANNEL_OPEN);

    // -- closing
    client.close(); // whole tcp connection from client to server is closed: i.e. application closed

    Waiter::wait([&] () { return serverControl.state() == Channel::CHANNEL_CLOSED; });
    EXPECT_EQ(serverControl.state(), Channel::CHANNEL_CLOSED);

    // On client side closing means tearing down all data structures.
    // Now checking that just signal arrives
    EXPECT_EQ(disconnectedReceived, 1);
    // TODO: when client connection goes down, all control should be recreated

    // -- closing
    server.close(); // just to be nice
    qDebug("### tearing down PingBothWays");
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

    PlayerMetadata playerMeta(100, "foobar");

    PlayerChannel* clientPlayer= serverSetup.channelManager.openPlayerChannel(playerMeta);
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() > 0; }, true);
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 1);

    // Waiting OpenChannelAccepted to arrive
    Waiter::wait([&] () { return clientPlayer->state() == Channel::CHANNEL_OPEN; }, true);

    // -- close

    serverSetup.channelManager.unregisterChannel(clientPlayer->channelId()); // sends closing message, no response
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() < 1; });
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 0);

    // closing happens in setup class desctructors
}


TEST(CommunicationIntegration, WhenClientDisconnectsAllServerSideChannelHandlersClosedAndReconnectedLater)
{
    ServerSetup serverSetup;
    serverSetup.start();

    ClientSetup* clientSetup = new ClientSetup();
    clientSetup->start();

    Waiter::wait([&] () { return clientSetup->tcpClient.isConnected(); });

    PlayerMetadata playerMeta(100, "FooPlayer");

    PlayerChannel* serverSidePlayer = serverSetup.channelManager.openPlayerChannel(playerMeta);

    // Waiting OpenChannelAccepted to arrive (OPEN_ONGOING -> OPEN)
    Waiter::wait([&] () { return serverSidePlayer->state() == Channel::CHANNEL_OPEN; }, true);

    // -- close
    // when serverside notices disconnect all channels should be closed
    clientSetup->tcpClient.close();
    Waiter::wait([&] () { return serverSidePlayer->state() == Channel::CHANNEL_CLOSED; }, true);
    ASSERT_TRUE(serverSidePlayer->state() == Channel::CHANNEL_CLOSED);
    ASSERT_TRUE(serverSetup.controlChannel.state() == Channel::CHANNEL_CLOSED);

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
    Waiter::wait([&] () { return serverSidePlayer->state() == Channel::CHANNEL_OPEN; }, true);
    ASSERT_TRUE(serverSidePlayer->state() == Channel::CHANNEL_OPEN);
    ASSERT_TRUE(serverSetup.controlChannel.state() == Channel::CHANNEL_OPEN);
    ASSERT_EQ(clientSetup->playersManager.numberOfPlayers(), 1);

    // -- tear down
    // closing happens in setup class desctructors
}


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

    PlayerMetadata playerMeta(100, "FooPlayer");
    PlayerChannel* serverSidePlayer = serverSetup.channelManager.openPlayerChannel(playerMeta);
    Waiter::waitAndAssert([&] ()
        { return serverSidePlayer->state() == Channel::CHANNEL_OPEN; }, true);

    int serverPlayerMessageReceived = 0;
    QByteArray serverPlayerMessage;
    QObject::connect(serverSidePlayer, &PlayerChannel::playerMessageReceived,
                     [&] (int channelId, const QByteArray msg) {
        Q_UNUSED(channelId);
        serverPlayerMessageReceived++; serverPlayerMessage = msg;
    });

    // --
    QByteArray serverMsg("serverMsg");
    qDebug("###########################");
    serverSidePlayer->sendPlayerMessage(serverMsg);
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
    Waiter::wait([&] () { return serverSidePlayer->state() == Channel::CHANNEL_CLOSED; });
}


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

    mobile::Application northApplication;

    bool consoleConnectionIsOpen = false;
    QObject::connect(&northApplication, &mobile::Application::consoleConnectionOpened,
                     [&] () { consoleConnectionIsOpen = true; });

    bool northSideClientMessageReceived = false;
    QString northSideClientMessage;
    QObject::connect(&northApplication, &mobile::Application::playerMessageReceived,
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
    ServerSetup serverSetup;
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

    mobile::Application northApplication1;

    QString northSideClient1Message;
    QObject::connect(&northApplication1, &mobile::Application::playerMessageReceived,
                     [&] (QString msg) { northSideClient1Message = msg; });

    mobile::Application northApplication2;

    QString northSideClient2Message;
    QObject::connect(&northApplication2, &mobile::Application::playerMessageReceived,
                     [&] (QString msg) { northSideClient2Message = msg; });

    mobile::Application northApplication3;

    QString northSideClient3Message;
    QObject::connect(&northApplication3, &mobile::Application::playerMessageReceived,
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
    WAIT_AND_ASSERT(serverSetup.controlChannel.state() == Channel::CHANNEL_CLOSED);

// -- new south side client
    southClientSetup = new ClientSetup();
    southClientSetup->start();

    playerMessageReceived = 0;
    QObject::connect(&(southClientSetup->playersManager), &PlayersManager::playerMessageReceived,
                     [&] (int playerId, QByteArray msg) {
        Q_UNUSED(playerId);
        playerMessageReceived++; playerInMessage = msg;
    });

    // reconnection occurs
    WAIT_AND_ASSERT(serverSetup.controlChannel.state() == Channel::CHANNEL_OPEN);
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
