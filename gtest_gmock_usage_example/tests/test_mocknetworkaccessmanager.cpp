#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;

#include <QDebug>

#include "namnuser.h"

#include "mock_qnetworkaccessmanager.h"


/* template approach doesn't work
TEST(mockQNetworkAccessManager, simple)
{
    // test normal creatio ok (compilation passes)
    MockNetworkAccessManager mock;

    //MockNetworkAccessManager* CreateNetworkAccessManager<class MockNetworkAccessManager>();

    //CreateNetworkAccessManager(&mock);

    //NAMNUser<MockNetworkAccessManager> namnuser;
    NAMNUser namnuser;
    PacketReader<QNetworkAccessManager> test;
    test.ReadPackets(NULL);

    PacketReader<MockNetworkAccessManager> test2;
    test2.ReadPackets(NULL);
    // TODO: test using get()
}
*/

TEST(mockQNetworkAccessManager, proxy)
{
    QNetworkAccessManager qnam;
    ProxyQNetworkAccessManager proxy(&qnam);
    MockProxyQNetworkAccessManager mockProxy;

    QUrl url("http://localhost:8000");
    QNetworkReply* r1 = proxy.get(QNetworkRequest(url));
    EXPECT_FALSE(r1->isFinished());

    StubQNetworkReply mockReply;
    EXPECT_CALL(mockProxy, get(_))
              .Times(1)
              .WillOnce( Return(&mockReply));

    // note get_proxytest() - this is temporary secondary way
    MockProxyQNetworkReply mockProxyReply;
    EXPECT_CALL(mockProxyReply, isFinished())
              .Times(1)
              .WillOnce( Return(true));

    EXPECT_CALL(mockProxy, get_proxytest(_))
              .Times(1)
              .WillOnce( Return(&mockProxyReply));

    QNetworkReply* r2 = mockProxy.get(QNetworkRequest(url));
    EXPECT_EQ(r2, &mockReply); // pointer comparison

    // testing other approach
    // 1st using standard proxy
    ProxyQNetworkReply* r3 = proxy.get_proxytest(QNetworkRequest(url));
    EXPECT_FALSE(r3->isFinished()); // default of not executed real http request

    // 2nd using mock
    ProxyQNetworkReply* r4 = mockProxy.get_proxytest(QNetworkRequest(url));
    EXPECT_TRUE(r4->isFinished());
    qDebug() << "END OF TEST";
}
