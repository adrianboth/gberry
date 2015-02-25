#include <gtest/gtest.h>

#include <QObject>

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "channelmanager.h"
#include "playercontrolchannel.h"
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
            [&] (int connectionId, int channelId, const QByteArray& msg) {
        Q_UNUSED(connectionId);
        serverChannelManager.processMessage(channelId, msg);
    });

    QObject::connect(&serverChannelManager, &ServerSideChannelManager::outgoingMessage,
                     [&server] (int channelId, const QByteArray& msg) {
        server.write(0, channelId, msg); // connection id count should have started from zero
    });

    ServerSideControlChannel serverControl;
    serverChannelManager.registerHandler(&serverControl);

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
    clientChannelManager.registerHandler(&clientControl);

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

    EXPECT_TRUE(clientControl.isConnectionValid());
    EXPECT_TRUE(serverControl.isConnectionValid());

    // other way round: server -> client
    serverControl.ping();

    Waiter::wait([&] () { return clientPingReceived > 1; });
    EXPECT_EQ(clientPingReceived, 2);

    // answer should come back
    Waiter::wait([&] () { return serverPingReceived > 1; });
    EXPECT_EQ(serverPingReceived, 2);

    // pings have put root channels to open state (at least not yet any handshaking
    EXPECT_EQ(serverControl.state(), ChannelHandler::CHANNEL_OPEN);
    EXPECT_EQ(clientControl.state(), ChannelHandler::CHANNEL_OPEN);

    // -- closing
    client.close(); // whole tcp connection from client to server is closed: i.e. application closed

    Waiter::wait([&] () { return serverControl.state() == ChannelHandler::CHANNEL_CLOSED; });
    EXPECT_EQ(serverControl.state(), ChannelHandler::CHANNEL_CLOSED);

    // On client side closing means tearing down all data structures.
    // Now checking that just signal arrives
    EXPECT_EQ(disconnectedReceived, 1);
    // TODO: when client connection goes down, all control should be recreated

    // -- closing
    server.close(); // just to be nice
    qDebug("### tearing down PingBothWays");
}


TEST(CommunicationIntegration, newPlayerChannel_createdAndDestroyed)
{
    ServerSetup serverSetup;
    serverSetup.start();

    // --
    ClientSetup clientSetup;
    clientSetup.start();

    Waiter::wait([&] () { return clientSetup.tcpClient.isConnected(); });
    // TODO: should we check connected() signal or is that done elsewhere

    // -- new player (remember server is 'comms')

    int playerChannelId = serverSetup.channelManager.nextFreeChannelId();
    PlayerControlChannel clientPlayer(playerChannelId);

    serverSetup.channelManager.registerHandler(&clientPlayer);
    serverSetup.channelManager.openChannel(playerChannelId);
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() > 0; }, true);
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 1);

    // Waiting OpenChannelAccepted to arrive
    Waiter::wait([&] () { return clientPlayer.state() == ChannelHandler::CHANNEL_OPEN; }, true);

    // -- close

    serverSetup.channelManager.unregisterHandler(clientPlayer.channelId()); // sends closing message, no response
    Waiter::wait([&] () { return clientSetup.playersManager.numberOfPlayers() < 1; });
    ASSERT_EQ(clientSetup.playersManager.numberOfPlayers(), 0);

    // closing happens in setup class desctructors
}


// TODO: test client tcp connection dead -> close all channels -> reconnecting

// TODO: connection out -> once reconnected restore
//  * channel states from OPEN -> CLOSED -> OPEN
