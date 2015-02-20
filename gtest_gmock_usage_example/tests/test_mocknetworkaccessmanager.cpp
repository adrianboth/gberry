#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;

#include <QDebug>

#include "namnuser.h"

#include "proxyqnetwork.h"
#include "mock_proxyqnetwork.h"

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
    // just dummy url - no response expected
    QUrl url("http://localhost:8000");

    QNetworkAccessManager qnam;
    ProxyQNetworkAccessManager proxy(&qnam);
    ProxyQNetworkReply* r1 = proxy.get(QNetworkRequest(url));
    EXPECT_FALSE(r1->isFinished()); // call is proxied to real QNetworkReply

    // -- similar case but now with mock objects
    MockProxyQNetworkAccessManager mockProxy;

    MockProxyQNetworkReply mockReply;
    EXPECT_CALL(mockProxy, get(_))
              .Times(1)
              .WillOnce( Return(&mockReply));

    EXPECT_CALL(mockReply, isFinished())
              .Times(1)
              .WillOnce( Return(true));

    ProxyQNetworkReply* r = mockProxy.get(QNetworkRequest(url));
    EXPECT_EQ(r, &mockReply); // pointer comparison
    EXPECT_TRUE(r->isFinished());
}
