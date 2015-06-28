#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QScopedPointer>

#include "server/application/applicationmeta.h"
#include "application.h"

#include "applicationcontroller.h"

#include "testobjects/stub_systemservices.h"

#include "testutils/waiter.h"
#include "testutils/util_enablelog.h"

// test fixture
class ApplicationControllerF : public ::testing::Test {
 protected:
  virtual void SetUp() {
    TestUtils::enabledStdoutLogging();
  }
};


TEST_F(ApplicationControllerF, LaunchOKWithAdvancedConstructor)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta()); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");
    ApplicationRegistry applicationRegistry;
    QSharedPointer<Application> app(new Application(meta));
    ApplicationController controller(qSharedPointerCast<IApplication>(app), &applicationRegistry);

    bool launched = false;
    QObject::connect(&controller, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&controller, &ApplicationController::stopped, [&] () { stopped = true; });

    controller.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    controller.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, LaunchOKWithDefaultConstructor)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta()); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");

    QSharedPointer<Application> app(new Application(meta));
    ApplicationController controller;
    controller.setApplication(qSharedPointerCast<IApplication>(app));

    bool launched = false;
    QObject::connect(&controller, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&controller, &ApplicationController::stopped, [&] () { stopped = true; });

    controller.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    controller.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, PauseAndResume)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");

    QSharedPointer<Application> app(new Application(meta));
    ApplicationController controller;
    controller.setApplication(qSharedPointerCast<IApplication>(app));

    bool launched = false;
    QObject::connect(&controller, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&controller, &ApplicationController::stopped, [&] () { stopped = true; });

    controller.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    controller.pause();
    QCoreApplication::processEvents();

    // TODO: somehow validate that pausing really occurred

    controller.resume();
    QCoreApplication::processEvents();

    // TODO: somehow validate that resume really occured

    controller.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, StopTakesSometimeAndLaunchIsDelayed)
{
    QScopedPointer<TestSystemServices> testservices(new TestSystemServices);
    testservices->registerInstance();

    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");
    QSharedPointer<Application> app(new Application(meta));

    ApplicationController controller;
    controller.setApplication(qSharedPointerCast<IApplication>(app));

    bool launched = false;
    QObject::connect(&controller, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&controller, &ApplicationController::stopped, [&] () { stopped = true; });


    controller.launch();
    Waiter::wait([&] () { return launched; });
    ASSERT_TRUE(launched);

    // -- TEST
    launched = false;

    controller.setProperty(ApplicationController::PROCESS_KILL_WAIT_MS_PROP, 20); // timer shorter for unit tests

    controller.stop(); // do not process events -> immediate launch
    controller.launch();
    QCoreApplication::processEvents();
    // --

    // timer should trigger after third round of event processing
    Waiter::wait([&] () { return stopped; });
    ASSERT_TRUE(stopped);

    testservices->invokeSingleshotTimer();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    // -- CLEANUP
    stopped = false;
    controller.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}
