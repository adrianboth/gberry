#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QScopedPointer>

#include "applicationmeta.h"
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
    ApplicationController app(meta);

    bool launched = false;
    QObject::connect(&app, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&app, &ApplicationController::stopped, [&] () { stopped = true; });

    app.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    app.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, LaunchOKWithDefaultConstructor)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta()); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");
    ApplicationController app;
    app.setApplication(meta);

    bool launched = false;
    QObject::connect(&app, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&app, &ApplicationController::stopped, [&] () { stopped = true; });

    app.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    app.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, PauseAndResume)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");

    ApplicationController app;
    app.setApplication(meta);

    bool launched = false;
    QObject::connect(&app, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&app, &ApplicationController::stopped, [&] () { stopped = true; });

    app.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    app.pause();
    QCoreApplication::processEvents();

    // TODO: somehow validate that pausing really occurred

    app.resume();
    QCoreApplication::processEvents();

    // TODO: somehow validate that resume really occured

    app.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}

TEST_F(ApplicationControllerF, StopTakesSometimeAndLaunchIsDelayed)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta); // this now linux only
    meta->setName("test");
    meta->setApplicationExecutablePath("/bin/bash");

    ApplicationController app;
    app.setApplication(meta);

    bool launched = false;
    QObject::connect(&app, &ApplicationController::launched, [&] () { launched = true; });

    bool stopped = false;
    QObject::connect(&app, &ApplicationController::stopped, [&] () { stopped = true; });

    app.launch();
    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    // -- TEST
    launched = false;
    QScopedPointer<TestSystemServices> testservices(new TestSystemServices);
    testservices->registerInstance();
    app.setProperty(ApplicationController::PROCESS_KILL_WAIT_MS_PROP, 20); // timer shorter for unit tests

    app.stop(); // do not process events -> immediate launch
    app.launch();
    QCoreApplication::processEvents();

    // timer should trigger after third round of event processing
    EXPECT_TRUE(stopped);
    testservices->invokeSingleshotTimer();

    Waiter::wait([&] () { return launched; });
    EXPECT_TRUE(launched);

    // -- CLEANUP
    app.stop();
    Waiter::wait([&] () { return stopped; });
    EXPECT_TRUE(stopped);
}
