#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::_;

#include <QCoreApplication>

#include "classundertest.h"

#include "mock_iotherservice.h"
#include "mock_iqsignal.h"

// TODO: better common location

// It's important that the << operator is defined in the SAME
// namespace that defines Bar.  C++'s look-up rules rely on that.
::std::ostream& operator<<(::std::ostream& os, const QString& bar) {
  return os << qPrintable(bar);
}


TEST(classUnderTest, publicFuncCall)
{
    // example comparison
    EXPECT_EQ(1000, 1000); // ok

    ClassUnderTest cut;
    QString ret = cut.publicFunc();

    // This doesn't work -> How to extend?
    //EXPECT_EQ(ret, "data1") << "Testing QString equality";

    // we can't use direct string comparison ...
    EXPECT_TRUE(ret == "publicFuncReturn") << "Got:" << ret;
}

TEST(classUnderTest, mockPublicFunc)
{
    MockIOtherService mock;
    EXPECT_CALL(mock, publicFunc())
          .Times(AtLeast(1))
          .WillOnce( Return(QString("mocked") ));

    ClassUnderTest cut(&mock);
    //mock.publicFunc();

    QString ret = cut.publicFunc();
    EXPECT_TRUE(ret == "mocked") << "Got:" << ret;
}


#include "signalutil.h"

TEST(classUnderTest, mockSlot)
{
    MockIQSignal mock;
    EXPECT_CALL(mock, slot1(_))
          .Times(1);

    StrictMock<MockIOtherService> strict_mock; // no calls allowed
    ClassUnderTest cut(&strict_mock);

    // Qt signal connection requires that we are inside QObject
    //  -> we need helper to connect here in test case
    //     (helper uses interfaces)
    SignalUtil util;
    util.connectSignals(&cut, &mock);

    cut.triggerSignal1();
    // event is not processed until we explictly do so
    QCoreApplication::processEvents();

}

TEST(classUnderTest, mockSignal)
{
    // use case is that if connect() is done for mock object

    MockIOtherService otherMock;
    EXPECT_CALL(otherMock, publicFunc2())
          .Times(AtLeast(1));

    ClassUnderTest cut(&otherMock);

    MockIQSignal mock; // this not called

    SignalUtil util;
    util.connectSignals2(&mock, &cut);

    // we could of course call slot directly but
    //  - we might not have visibility to it (class structure)
    //  - we want to test that connect() has been really called
    //cut.slot1("call from testcase");

    // so alternative is to use helper function in mock class
    mock.emitSignal2("emit through mock");
}

// TODO: mock concrete class
