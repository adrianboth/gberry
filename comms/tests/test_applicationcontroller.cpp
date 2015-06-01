#include <gtest/gtest.h>

#include <QCoreApplication>

#include "applicationmeta.h"
#include "applicationcontroller.h"

#include "testutils/waiter.h"

#include "testutils/util_enablelog.h"

TEST(ApplicationController, LaunchOKWithAdvancedConstructor)
{
    TestUtils::enabledStdoutLogging();

    QPointer<ApplicationMeta> meta = new ApplicationMeta("test", "/bin/bash"); // this now linux only
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

TEST(ApplicationController, LaunchOKWithDefaultConstructor)
{
    TestUtils::enabledStdoutLogging(); // TODO: generic setup

    QPointer<ApplicationMeta> meta = new ApplicationMeta("test", "/bin/bash"); // this now linux only
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

TEST(ApplicationController, PauseAndResume)
{
    TestUtils::enabledStdoutLogging(); // TODO: generic setup

    QPointer<ApplicationMeta> meta = new ApplicationMeta("test", "/bin/bash"); // this now linux only
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
