
#include <gtest/gtest.h>

#include <QCoreApplication>

#include "applicationmeta.h"
#include "launchcontroller.h"

#include "mocks/mock_iapplications.h"

#include "testutils/waiter.h"

#include "testutils/util_enablelog.h"
namespace {
    static TestUtils::StdoutLoggingEnabler& logging = TestUtils::enabledStdoutLogging();
}

TEST(LaunchController, LaunchController)
{
    QMap<QString, QSharedPointer<ApplicationMeta>> appsMap;
    StubIApplications apps(appsMap);
    LaunchController controller(&apps);

    bool launchCalled = false;
    QObject::connect(&controller, &LaunchController::launched, [&] () { launchCalled = true; });

    bool launchFailedCalled = false;
    QObject::connect(&controller, &LaunchController::launchFailed, [&] () { launchFailedCalled = true; });

    bool stoppedCalled = false;
    QObject::connect(&controller, &ApplicationController::stopped, [&] () { stoppedCalled = true; });

    // -- no app defined
    controller.launch();
    Waiter::wait([&] () { return launchFailedCalled; });
    EXPECT_TRUE(launchFailedCalled);
    EXPECT_FALSE(launchCalled);

    // -- invalid app defined
    EXPECT_FALSE(controller.useApplication("notfound"));

    launchFailedCalled = false;
    controller.launch();
    Waiter::wait([&] () { return launchFailedCalled; });
    EXPECT_TRUE(launchFailedCalled);
    EXPECT_FALSE(launchCalled);

    // -- valid app
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta); // this now linux only
    meta->setApplicationId("test");
    meta->setApplicationExecutablePath("/bin/bash");
    appsMap["test-1.0"] = meta;

    EXPECT_TRUE(controller.useApplication("test-1.0"));

    launchFailedCalled = false;
    controller.launch();
    Waiter::wait([&] () { return launchCalled; });
    EXPECT_FALSE(launchFailedCalled);
    EXPECT_TRUE(launchCalled);

    controller.stop();
    Waiter::wait([&] () { return stoppedCalled; });
    EXPECT_TRUE(stoppedCalled);
}
