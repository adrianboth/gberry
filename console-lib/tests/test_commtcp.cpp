#include <gtest/gtest.h>

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include "server/commtcpserver.h"
#include "client/commtcpclient.h"

#include "utils/testtcpserver.h"

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>


TEST(CommTcp, connectAndDisconnect)
{
    CommTcpServer server(7000);

    SignalRecorder connectedRecorder;
    SignalRecorder disconnectedRecorder;
    QObject::connect(&server,            &CommTcpServer::connected,
                     &connectedRecorder, &SignalRecorder::signal1_int);
    QObject::connect(&server,               &CommTcpServer::disconnected,
                     &disconnectedRecorder, &SignalRecorder::signal1_int);

    server.open();

    // during test we can use blocking calls
    QTcpSocket client;
    client.connectToHost(QHostAddress::LocalHost, 7000);

    auto func = [&] () { return connectedRecorder.received(); };
    Waiter waiter(func);
    waiter.wait();

    ASSERT_TRUE(connectedRecorder.received()); // this assertion could be already in waiter
    ASSERT_TRUE(client.isOpen());

    EXPECT_FALSE(disconnectedRecorder.received());

    connectedRecorder.reset();

    client.close();
    auto func2 = [&] () { return disconnectedRecorder.received_int(); };
    Waiter waiter2(func2);
    waiter2.wait();

    ASSERT_TRUE(disconnectedRecorder.received_int());
    EXPECT_FALSE(connectedRecorder.received());

    // TODO: check channel id (TODO: new name for "channel")

}


TEST(CommTcp, writeDataToServer)
{
    CommTcpServer server(7000);

    int called = 0;
    int ourId = -1;
    QString ourStr;

    auto func = [&] (int connectionId, int cid, QByteArray msg) {
        Q_UNUSED(connectionId);
        called++; ourId = cid; ourStr = QString(msg);
    };
    QObject::connect(&server, &CommTcpServer::received, func);

    server.open();

    // be cause there might be data going somewhere
    // it is not good to destroy QTcpSocket directly
    QTcpSocket* client = new QTcpSocket();
    QObject::connect(client, &QTcpSocket::disconnected,
                     client, &QTcpSocket::deleteLater);

    client->connectToHost(QHostAddress::LocalHost, 7000);

    Waiter::wait([&client] () { return client->isOpen(); }, true);

    qDebug("Going to send data for CommTcpServer");
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << (quint32)0; // reserve space for message length information
    out << (quint32)1; // channel 1
    out << "ping";
    out.device()->seek(0);
    out << (quint32)(data.size() - sizeof(quint32)*2); // write length

    client->write(data);

    qDebug("Waiting: called > 0");
    Waiter::wait([called] () { return called > 0; }, true);

    // we don't now really care id, just something got
    EXPECT_NE(ourId, -1);
    EXPECT_TRUE(ourStr == "ping") << "Got: "<< ourStr;

    // cleanup
    client->close();
}


TEST(CommTcp, clientReadsData)
{
    CommTcpClient client(7777);

    int called = 0;
    int channelId = -1;
    QString ourStr;
    auto func = [&] (int cid, const QByteArray msg) { called++; channelId = cid; ourStr = QString(msg); };
    QObject::connect(&client, &CommTcpClient::received, func);

    TestTcpServer server(7777);

    client.open();

    ASSERT_TRUE(server.waitForConnection(1000)); // this accepts connections
    Waiter::wait([&client] () { return client.isConnected(); }, true);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    out << (quint32)0 << (quint32) 1 << "ping";
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    server.socket->write(block);
    Waiter::wait([&] () { return called > 0; }, true);

    EXPECT_EQ(called, 1);
    EXPECT_TRUE(ourStr == "ping") << "Got: " << ourStr;

    // -- closing
    client.close();
}


TEST(CommTcp, clientAndServer)
{
    CommTcpServer server(7000);

    int serverReceived = 0;
    int connectionId = -1;
    int serverChannelId = -1;
    QString serverStr;
    auto serverFunc = [&] (int connId, int cid, QByteArray msg) {
        connectionId = connId; serverChannelId = cid; serverReceived++; serverStr = QString(msg);
    };
    QObject::connect(&server, &CommTcpServer::received, serverFunc);

    server.open();

    // --
    CommTcpClient client(7000);

    int clientReceived = 0;
    int clientChannelId = -1;
    QString clientStr;
    auto clientFunc = [&] (int cid, const QByteArray msg) {
        clientReceived++; clientChannelId = cid; clientStr = QString(msg);
    };
    QObject::connect(&client, &CommTcpClient::received, clientFunc);

    client.open();
    Waiter::wait([&client] () { return client.isConnected(); });

    // --
    QByteArray foo("foo");
    client.write(1, foo); // we just "know" channel id = 1
    Waiter::wait([&] () { return serverReceived > 0; });
    EXPECT_TRUE(serverReceived > 0);
    EXPECT_TRUE(serverStr == "foo") << "Got: " << serverStr;

    // --
    QByteArray bar("bar");
    server.write(connectionId, serverChannelId, bar);
    Waiter::wait([&] () { return clientReceived > 0; });
    EXPECT_TRUE(clientReceived > 0);
    EXPECT_TRUE(clientStr == "bar") << "Got: " << clientStr;

    EXPECT_EQ(serverChannelId, clientChannelId);
    //--
    server.close();
    client.close();
}

// TODO: test multiple messages going towards server

// TODO: test reconnect (client + server)
