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

#include "server/application/applicationmeta.h"
#include "application.h"
#include "launchcontroller.h"

#include "mocks/mock_iapplications.h"

#include <testutils/waiter.h>
#include "testobjects/simpleapplicationexecutionsetup.h"


TEST(LaunchController, LaunchController)
{
    QMap<QString, QSharedPointer<IApplication>> appsMap;
    StubIApplications apps(appsMap);
    SimpleApplicationExecutionSetup executionSetup;
    LaunchController controller(&apps, &executionSetup);

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

    QSharedPointer<Application> app(new Application(meta));
    appsMap["test-1.0"] = qSharedPointerCast<IApplication>(app);

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
