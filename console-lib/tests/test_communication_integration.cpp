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
