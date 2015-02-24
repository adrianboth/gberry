#include <gtest/gtest.h>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "messagereader.h"


TEST(MessageReader, ReadSingleMessage)
{
    QTcpServer server;
    ASSERT_TRUE(server.listen(QHostAddress::LocalHost, 7000));

    QTcpSocket* clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, &QTcpSocket::disconnected,
                     clientSocket, &QTcpSocket::deleteLater);

    clientSocket->connectToHost(QHostAddress::LocalHost, 7000);

    ASSERT_TRUE(server.waitForNewConnection(3000));

    QTcpSocket* serverSocket = server.nextPendingConnection();
    QObject::connect(serverSocket, &QTcpSocket::disconnected,
                     serverSocket, &QTcpSocket::deleteLater);

    MessageReader reader(serverSocket);

    int channelId = -1;
    QString myStr;
    QObject::connect(&reader, &MessageReader::received,
                     [&] (int cid, const QByteArray& msg) { channelId = cid; myStr = QString(msg); } );

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint32)0 << (quint32) 1 << "ping";
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    clientSocket->write(block);
    Waiter::wait([&] () { return channelId != -1; });
    EXPECT_EQ(channelId, 1);
    EXPECT_TRUE(myStr == "ping") << "Got: " << myStr;

    // -- second time
    block.clear();
    out.device()->seek(0);
    // using 'channelId' as easy marked from invoked slot
    channelId = -1;

    out << (quint32)0 << (quint32) 1 << "pingreply";
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    clientSocket->write(block);
    Waiter::wait([&] () { return channelId != -1; });
    EXPECT_EQ(channelId, 1);
    EXPECT_TRUE(myStr == "pingreply") << "Got: " << myStr;

    // -- cleanup
    clientSocket->close();
}

TEST(MessageReader, ReadAndWrite)
{
    // TODO: put to common fixture
    QTcpServer server;
    ASSERT_TRUE(server.listen(QHostAddress::LocalHost, 7000));

    QTcpSocket* clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, &QTcpSocket::disconnected,
                     clientSocket, &QTcpSocket::deleteLater);

    clientSocket->connectToHost(QHostAddress::LocalHost, 7000);

    ASSERT_TRUE(server.waitForNewConnection(3000));

    QTcpSocket* serverSocket = server.nextPendingConnection();
    QObject::connect(serverSocket, &QTcpSocket::disconnected,
                     serverSocket, &QTcpSocket::deleteLater);

    MessageReader serverReader(serverSocket);
    MessageReader clientReader(clientSocket);

    int serverReceived = 0;
    QString serverStr;
    QObject::connect(&serverReader, &MessageReader::received,
                     [&] (int cid, const QByteArray& msg) {
        Q_UNUSED(cid); serverReceived++; serverStr = QString(msg);
    } );

    int clientReceived = 0;
    QString clientStr;
    QObject::connect(&clientReader, &MessageReader::received,
                     [&] (int cid, const QByteArray& msg) {
        Q_UNUSED(cid); clientReceived++; clientStr = QString(msg);
    } );

    // -- test
    QByteArray pingMsg("ping");
    QByteArray pingReplyMsg("pingreply");

    clientReader.write(1, pingMsg);

    Waiter::wait([&] () { return serverReceived > 0; });
    ASSERT_EQ(serverReceived, 1);
    EXPECT_TRUE(serverStr == "ping") << "Got: " << serverStr;

    serverReader.write(1, pingReplyMsg);

    Waiter::wait([&] () { return clientReceived > 0; });
    ASSERT_EQ(clientReceived, 1);
    EXPECT_TRUE(clientStr == "pingreply") << "Got: " << clientStr;

    // otherway round
    serverReader.write(1, pingMsg);

    Waiter::wait([&] () { return clientReceived > 1; });
    ASSERT_EQ(clientReceived, 2);
    EXPECT_TRUE(clientStr == "ping") << "Got: " << clientStr;

    clientReader.write(1, pingReplyMsg);

    Waiter::wait([&] () { return serverReceived > 1; });
    ASSERT_EQ(serverReceived, 2);
    EXPECT_TRUE(serverStr == "pingreply") << "Got: " << serverStr;


}

TEST(MessageReader, MultipleMessageIncoming)
{
    // case where because of delay in reading there is in fact multiple
    // messages availble to be read
    QTcpServer server;
    ASSERT_TRUE(server.listen(QHostAddress::LocalHost, 7000));

    QTcpSocket* clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, &QTcpSocket::disconnected,
                     clientSocket, &QTcpSocket::deleteLater);

    clientSocket->connectToHost(QHostAddress::LocalHost, 7000);

    ASSERT_TRUE(server.waitForNewConnection(3000));

    QTcpSocket* serverSocket = server.nextPendingConnection();
    QObject::connect(serverSocket, &QTcpSocket::disconnected,
                     serverSocket, &QTcpSocket::deleteLater);

    MessageReader reader(serverSocket);

    int received = 0;
    QStringList myStrings;
    QObject::connect(&reader, &MessageReader::received,
                     [&] (int cid, const QByteArray& msg) {
        Q_UNUSED(cid); received++; myStrings.append(QString(msg)); } );

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint32)0 << (quint32) 1 << "ping1";
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    int bytesInFirstMessage = block.size();
    out.device()->seek(bytesInFirstMessage);

    // second message
    out << (quint32)0 << (quint32) 1 << "ping2";
    out.device()->seek(bytesInFirstMessage); // to beginning of 2nd msg
    out << (quint32)(block.size() - bytesInFirstMessage  - sizeof(quint32)*2);

    clientSocket->write(block);
    Waiter::wait([&] () { return received > 0; });
    ASSERT_EQ(received, 2);
    ASSERT_EQ(myStrings.length(), 2);
    EXPECT_TRUE(myStrings.at(0) == "ping1") << "Got: " << myStrings.at(0);
    EXPECT_TRUE(myStrings.at(1) == "ping2") << "Got: " << myStrings.at(1);
}

TEST(MessageReader, ReadInMultipleParts)
{
    // case where only part of message has been transfered
    QTcpServer server;
    ASSERT_TRUE(server.listen(QHostAddress::LocalHost, 7000));

    QTcpSocket* clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, &QTcpSocket::disconnected,
                     clientSocket, &QTcpSocket::deleteLater);

    clientSocket->connectToHost(QHostAddress::LocalHost, 7000);

    ASSERT_TRUE(server.waitForNewConnection(3000));

    QTcpSocket* serverSocket = server.nextPendingConnection();
    QObject::connect(serverSocket, &QTcpSocket::disconnected,
                     serverSocket, &QTcpSocket::deleteLater);

    MessageReader reader(serverSocket);

    int received = 0;
    QString myString;
    QObject::connect(&reader, &MessageReader::received,
                     [&] (int cid, const QByteArray& msg) {
        Q_UNUSED(cid); received++; myString = QString(msg); } );

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint32)0 << (quint32) 1 << "ping1";
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    int bytes = block.size();

    // we have now whole message, write it in parts
    clientSocket->write(block.mid(0, 2)); // just first two bytes from length
    Waiter::wait([&] () { return received > 0; }); // should not occur
    ASSERT_EQ(received, 0);

    clientSocket->write(block.mid(2, 4)); // two bytes from length + channel
    Waiter::wait([&] () { return received > 0; });
    ASSERT_EQ(received, 0);

    clientSocket->write(block.mid(6, 2)); // rest of channel bytes
    Waiter::wait([&] () { return received > 0; });
    ASSERT_EQ(received, 0);

    clientSocket->write(block.mid(8, 2)); // two bytes from data
    Waiter::wait([&] () { return received > 0; });
    ASSERT_EQ(received, 0);

    clientSocket->write(block.mid(10, bytes - 10)); // rest of data
    Waiter::wait([&] () { return received > 0; });
    ASSERT_EQ(received, 1);
    EXPECT_TRUE(myString == "ping1") << "Got: " << myString;
}
