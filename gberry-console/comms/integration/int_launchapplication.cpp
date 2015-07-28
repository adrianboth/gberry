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

#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTemporaryDir>

#include "server/serversidecontrolchannel.h"
#include "realsystemservices.h"

#include "localapplicationsstorage.h"
#include "localapplications.h"
#include "applicationcontroller.h"
#include "server/channelfactory.h"
#include "utils/fileutils.h"
#include "applicationexecutionsetup.h"


#include <server/serversetup.h>
#include <server/playersessionmanager.h>
#define LOG_AREA "IntegrationLaunchApplication"
#include "log/log.h"

#include "testutils/util_testdata.h"
#include "testobjects/stub_channelsouthpartner.h"

using namespace GBerry;
using namespace GBerryConsole::Test;

// launches real app and waits connection
TEST(IntegrationLaunchApplication, OK)
{
    // we expect to apps dir -> we make copy of it because we are writing there
    QString appsDir = TestUtils::testdataDirPath("testcase1");
    QTemporaryDir temporaryDir;
    DEBUG("Copy testdata from" << appsDir << "to" << temporaryDir.path());
    ASSERT_TRUE(GBerryLib::copyRecursively(appsDir, temporaryDir.path())); // verify ok

    // we need a working binary -> assuming one is compiled to deploy/ -> copy it
    // TODO: replace with something else
    QString rootPath = "/home/tevuore/workspace/gberry-projects/gberry-bitbucket/builds/deploy-console-Desktop_Qt_5_4_0_GCC_64bit-Debug";
    QString exePath = GBerryLib::joinpath({rootPath, "apps", "testapp", "testapp"});
    QFile exeFile(exePath);
    ASSERT_TRUE(exeFile.exists());

    QString dstExePath = GBerryLib::joinpath({temporaryDir.path(), "apps", "testapp-1.0", "testapp"});
    ASSERT_TRUE(exeFile.copy(dstExePath));

    // --

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;

    LocalApplicationsStorage appStorage(joinpath(temporaryDir.path(), "apps"));
    QSharedPointer<LocalApplications> apps(new LocalApplications(&appStorage));

    QSharedPointer<IApplications> iapps(qSharedPointerCast<IApplications>(apps));

    ChannelFactory channelFactory;
    setup.use(&channelFactory);
    setup.setup();

    QSharedPointer<IApplication> testapp = iapps->application("testapp-1.0");
    ASSERT_FALSE(testapp.isNull());

    // -- apps execution env setup
    QString qtlibsPath = GBerryLib::joinpath({rootPath, "lib", "Qt"});
    QtLibrariesManager qtlibsManager(qtlibsPath);

    ApplicationExecutionSetup executionSetup(&qtlibsManager, &setup.applicationRegistry, true); // enablelogging=true

    ApplicationController testAppController(testapp, &executionSetup);

    bool applicationConnectedEmitted = false;
    bool applicationDisconnectedEmitted = false;

    QObject::connect(&setup.tcpServer, &CommTcpServer::connected,
                     [&] (int connectionId) { Q_UNUSED(connectionId); applicationConnectedEmitted = true; });

    QObject::connect(&setup.tcpServer, &CommTcpServer::disconnected,
                     [&] (int connectionId) { Q_UNUSED(connectionId); applicationDisconnectedEmitted = true; });

    setup.start();
    bool keepWaiting = true;
    bool launchedEmitted = false;
    bool launchFailedEmitted = false;
    QObject::connect(&testAppController, &ApplicationController::launched,
                     [&] () { launchedEmitted = true; keepWaiting = false; });
    QObject::connect(&testAppController, &ApplicationController::launchFailed,
                     [&] () { launchFailedEmitted = true; keepWaiting = false; });

    testAppController.launch();

    // keep waiting launch has occurred
    WAIT_WITH_TIMEOUT(keepWaiting == false, 10000);
    ASSERT_TRUE(launchedEmitted);
    ASSERT_FALSE(launchFailedEmitted);

    // next wait connection is formed succesfully
    WAIT_CUSTOM_AND_ASSERT(setup.connectionManager->activeConnection(), 10000, 250);
    ASSERT_TRUE(applicationConnectedEmitted);
    ASSERT_FALSE(applicationDisconnectedEmitted);

    // TODO: send pingpong message to app
    //setup.channelManager->


    // kill application
    testAppController.stop();

    WAIT_CUSTOM_AND_ASSERT(!setup.connectionManager->activeConnection(), 10000, 250);
    ASSERT_TRUE(applicationDisconnectedEmitted);
}
