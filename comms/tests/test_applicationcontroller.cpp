#include <gtest/gtest.h>

#include <QCoreApplication>

#include "applicationmeta.h"
#include "applicationcontroller.h"

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
