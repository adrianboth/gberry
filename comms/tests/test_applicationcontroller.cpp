/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include <gtest/gtest.h>

#include <QCoreApplication>
#include <QScopedPointer>
#include <QTemporaryDir>

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

    QTemporaryDir tempDir; // dir created right away
    QFile myscriptFile(tempDir.path() + "/testscript.sh");
    ASSERT_TRUE(myscriptFile.open(QIODevice::WriteOnly));
    myscriptFile.write("!#/bin/bash\nsleep 60");
    myscriptFile.flush();
    myscriptFile.close();
    ASSERT_TRUE(myscriptFile.setPermissions(QFileDevice::ExeOwner | QFileDevice::WriteOwner | QFileDevice::ReadUser));
    qDebug() << "SCRIPT:" << myscriptFile.fileName();

    meta->setApplicationExecutablePath(myscriptFile.fileName());
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
