#include <testutils/qtgtest.h>
#include <QCoreApplication>

#include "utils/util_controllerproxy.h"

#include "uiappstatemachine.h"
#include "uiappstatemachine_private.h"

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "mocks/mock_iapplicationcontroller.h"


TEST(UIAppStateMachine, OKFlow)
{
    MockIApplicationController waitAppMock;
    MockIApplicationController mainuiMock;
    MockILaunchController      currentAppMock;
    UIAppStateMachine st(&waitAppMock, &mainuiMock, &currentAppMock);

    auto verifyMocks = [&] () { Mock::VerifyAndClearExpectations(&waitAppMock);
                                Mock::VerifyAndClearExpectations(&mainuiMock);
                                Mock::VerifyAndClearExpectations(&currentAppMock); };

    auto processEvents = [&] (int times=1) { for (int i=0; i < times; i++) { QCoreApplication::processEvents(); } };

    // --
    EXPECT_CALL(waitAppMock, launch()).Times(1);

    st.start();
    QCoreApplication::processEvents(); verifyMocks();
    EXPECT_TRUE(st.debugCurrentStateName() == "startup");

    // --
    EXPECT_CALL(mainuiMock, launch()).Times(1);
    //waitAppMock.emitLaunched(); // only in simulated mode
    st.applicationConnectionValidated("waitapp");
    QCoreApplication::processEvents(); verifyMocks();

    // --
    // no transition until explitly asked
    EXPECT_CALL(waitAppMock, pause()).Times(1);
    //mainuiMock.emitLaunched();
    st.applicationConnectionValidated("mainui");
    processEvents(); verifyMocks();

    // --
    // starting new app first showing wait screen
    EXPECT_CALL(waitAppMock, resume()).Times(1);
    EXPECT_CALL(mainuiMock, stop()).Times(1);
    QString id("foobarAppId");
    EXPECT_CALL(currentAppMock, useApplication(id)).Times(1);
    EXPECT_CALL(currentAppMock, launch()).Times(1);

    st.lauchApplication("foobarAppId"); // id
    processEvents(); verifyMocks();

    // --
    // new app running
    EXPECT_CALL(waitAppMock, pause()).Times(1);
    //currentAppMock.emitLaunched();
    st.applicationConnectionValidated("foobarAppId");
    processEvents(); verifyMocks();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_APP);

    // --
    // menu requested
    EXPECT_CALL(waitAppMock, resume()).Times(1);
    EXPECT_CALL(currentAppMock, fullApplicationId())
            .Times(1)
            .WillOnce(Return(QString("foobarAppId")));
    EXPECT_CALL(currentAppMock, stop()).Times(1);
    EXPECT_CALL(mainuiMock, launch()).Times(1);
    st.exitApplication("foobarAppId"); // and show menu
    processEvents(); verifyMocks();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_MAINUI) << st.debugCurrentStateName();

    // --
    EXPECT_CALL(waitAppMock, pause()).Times(1);
    //mainuiMock.emitLaunched();
    st.applicationConnectionValidated("mainui");
    processEvents(); verifyMocks();

    // finally mainui visible again
}

TEST(UIAppStateMachine, ApplicationDiedFlow)
{
    // driving first state to app visible and then starting to test

    MockIApplicationController waitAppMock;
    MockIApplicationController mainuiMock;
    MockILaunchController      currentAppMock;

    ::testing::NiceMock<MockIApplicationController> niceWaitAppMock;
    ::testing::NiceMock<MockIApplicationController> niceMainUIMock;
    ::testing::NiceMock<MockILaunchController> niceCurrentAppMock;

    ProxyIApplicationController waitAppMockProxy(&niceWaitAppMock);
    ProxyIApplicationController mainuiMockProxy(&niceMainUIMock);
    ProxyILaunchController currentAppMockProxy(&niceCurrentAppMock);

    UIAppStateMachine st(&waitAppMockProxy, &mainuiMockProxy, &currentAppMockProxy);

    auto verifyMocks = [&] () { Mock::VerifyAndClearExpectations(&waitAppMock);
                                Mock::VerifyAndClearExpectations(&mainuiMock);
                                Mock::VerifyAndClearExpectations(&currentAppMock); };

    auto processEvents = [&] (int times=1) { for (int i=0; i < times; i++) { QCoreApplication::processEvents(); } };

    // --
    // (this case also verifies that we can really drive states quickly where we want)
    st.start();
    processEvents();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_STARTUP);
    //niceWaitAppMock.emitLaunched(); // only in simulated mode
    st.applicationConnectionValidated("waitapp");
    processEvents();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_MAINUI);
    //niceMainUIMock.emitLaunched();
    st.applicationConnectionValidated("mainui");
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_MAINUI);
    processEvents();
    st.lauchApplication("foobarAppId"); // id
    processEvents();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_APP);
    //niceCurrentAppMock.emitLaunched();
    st.applicationConnectionValidated("foobarAppId");
    processEvents();
    ASSERT_TRUE(st.debugCurrentStateName() == STATE_APP);

    // now in app started state -> changes mock
    waitAppMockProxy.setTarget(&waitAppMock);
    mainuiMockProxy.setTarget(&mainuiMock);
    currentAppMockProxy.setTarget(&currentAppMock);

    // -- app will exit (die)
    EXPECT_CALL(waitAppMock, resume()).Times(1);
    EXPECT_CALL(mainuiMock, launch()).Times(1);
    EXPECT_CALL(currentAppMock, stop()).Times(1); // just in case
    currentAppMock.emitDied();
    processEvents(); verifyMocks();


    // finally mainui visible again
}

TEST(UIAppStateMachine, ApplicationLaunchFailedImmediately)
{
    // first driving to mainui
    MockIApplicationController waitAppMock;
    MockIApplicationController mainuiMock;
    MockILaunchController      currentAppMock;

    ::testing::NiceMock<MockIApplicationController> niceWaitAppMock;
    ::testing::NiceMock<MockIApplicationController> niceMainUIMock;
    ::testing::NiceMock<MockILaunchController> niceCurrentAppMock;

    ProxyIApplicationController waitAppMockProxy(&niceWaitAppMock);
    ProxyIApplicationController mainuiMockProxy(&niceMainUIMock);
    ProxyILaunchController currentAppMockProxy(&niceCurrentAppMock);

    UIAppStateMachine st(&waitAppMockProxy, &mainuiMockProxy, &currentAppMockProxy);

    auto verifyMocks = [&] () { Mock::VerifyAndClearExpectations(&waitAppMock);
                                Mock::VerifyAndClearExpectations(&mainuiMock);
                                Mock::VerifyAndClearExpectations(&currentAppMock); };

    auto processEvents = [&] (int times=1) { for (int i=0; i < times; i++) { QCoreApplication::processEvents(); } };

    // --
    // (this case also verifies that we can really drive states quickly where we want)
    st.start();
    processEvents();
    EXPECT_TRUE(st.debugCurrentStateName() == STATE_STARTUP);
    //niceWaitAppMock.emitLaunched(); // only in simulated mode
    st.applicationConnectionValidated("waitapp");
    processEvents();
    EXPECT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_MAINUI);
    //niceMainUIMock.emitLaunched();
    st.applicationConnectionValidated("mainui");
    EXPECT_TRUE(st.debugCurrentStateName() == STATE_MAINUI);
    processEvents();
    st.lauchApplication("foobarAppId"); // id
    processEvents();
    EXPECT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_APP);

    // take nice mocks away -> real mocks into play
    waitAppMockProxy.setTarget(&waitAppMock);
    mainuiMockProxy.setTarget(&mainuiMock);
    currentAppMockProxy.setTarget(&currentAppMock);

    // TODO: waitMock called?
    EXPECT_CALL(mainuiMock, launch()).Times(1);

    currentAppMock.emitLaunchFailed();
    processEvents();
    verifyMocks();
    EXPECT_TRUE(st.debugCurrentStateName() == STATE_WAITAPP_LAUNCHING_MAINUI);

}

// TODO: some kind of message from launch failed?
