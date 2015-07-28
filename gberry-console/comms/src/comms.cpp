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
 
 #include "comms.h"

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QSharedPointer>

#include <signal.h>

#include "localapplicationsstorage.h"
#include "localapplications.h"
#include "applicationcontroller.h"
#include "launchcontroller.h"
#include "uiappstatemachine.h"
#include "commschannelfactory.h"
#include "commsparameters.h"
#include "headserverconnection.h"
#include "invocationfactoryimpl.h"
#include "systemservices.h"
#include "utils/fileutils.h"
#include "commands/commscommands.h"
#include "commsconfig.h"
#include "applicationexecutionsetup.h"

#include <invocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>
#include <server/commtcpserver.h>

#include <server/serversetup.h>
#include <server/playersessionmanager.h>

#define LOG_AREA "Comms"
#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"


using namespace Comms;
using namespace GBerryLib;

namespace GBerryComms {

Comms::Comms()
{
}

Comms::~Comms()
{
}

int Comms::run(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("comms");
    QCoreApplication::setApplicationVersion("0.1");

    // --
    EnvironmentVariables env(QProcessEnvironment::systemEnvironment());
    CommsParameters params(env);
    params.parse(app.arguments());

    // --
    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    INFO("Setting up ...");

    CommsConfig commscfg(&params);

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;

    QString rootPath(params.value(CommsParameters::ROOT_PATH));
    DEBUG("Root path:" << rootPath);

    LocalApplicationsStorage appStorage(joinpath(rootPath, "apps"));
    QSharedPointer<LocalApplications> apps(new LocalApplications(&appStorage));

    QSharedPointer<IApplications> iapps(qSharedPointerCast<IApplications>(apps));

    InvocationFactoryImpl invocationFactory;
    QString urlPrefix("http://");
    urlPrefix.append(commscfg.value("headserver_host", "localhost"));
    urlPrefix.append(commscfg.value("headserver_url_prefix", "not defined"));
    INFO("Using HeadServer:" << urlPrefix);
    invocationFactory.setProperty("url_prefix", urlPrefix);

    GBerry::HeadServerConnection headServerConnection(&invocationFactory);

    DownloadableApplicationCache downloadableApplicationCache;
    CommsCommands commands(&appStorage,
                           &setup.applicationRegistry,
                           &headServerConnection,
                           &downloadableApplicationCache);
    CommsChannelFactory channelFactory(&setup.applicationRegistry, &commands);
    setup.use(&channelFactory);
    // create objects for additional setup
    setup.setup();


    auto getapp = [&] (const QString& appId) {
        QList<QSharedPointer<IApplication>> foundApps = iapps->applicationsByApplicationId(appId);
        // TODO: we good do something nice, during development time without waitapp and mainui is ok
        Q_ASSERT(foundApps.length() > 0);

        // TODO: we should support multiple versions
        if (foundApps.length() > 1)
            WARN("Multiple versions found for application" << appId << ". Selecting first one.");

        return foundApps[0];
    };

    // -- apps execution env setup
    QString qtlibsPath = GBerryLib::joinpath({rootPath, "lib", "Qt"});
    QtLibrariesManager qtlibsManager(qtlibsPath);

    DEBUG("Initializing configuration for 'waitapp' and 'mainui'");
    ApplicationExecutionSetup executionSetup(&qtlibsManager, &setup.applicationRegistry, true); // enablelogging=true

    ApplicationController waitAppController(getapp("waitapp"), &executionSetup);

    QObject::connect(&waitAppController, &ApplicationController::paused,
                     [&] () {

        int connectionId = setup.applicationRegistry.connectionIdByApplicationId(waitAppController.application()->meta()->applicationId());
        if (connectionId != -1) {
            setup.channelManager->deactivateConnection(connectionId);
        } else {
            WARN("'waitapp' paused but connection id not found from ApplicationRegistry. Can't deactivate connection.");
        }
    });

    QObject::connect(&waitAppController, &ApplicationController::resumed,
                     [&] () {

        int connectionId = setup.applicationRegistry.connectionIdByApplicationId(waitAppController.application()->meta()->applicationId());
        if (connectionId != -1) {
            setup.channelManager->activateConnection(connectionId);
        } else {
            WARN("'waitapp' resumed but connection id not found from ApplicationRegistry. Can't activate connection.");
        }
    });

    ApplicationController mainuiController(getapp("mainui"), &executionSetup);

    // TODO: using safe pointer goes too far ...
    LaunchController currentAppController(apps.data(), &executionSetup);

    UIAppStateMachine stateMachine(&waitAppController, &mainuiController, &currentAppController);

    QObject::connect(&commands, &CommsCommands::launchApplicationRequested,
                     &stateMachine, &UIAppStateMachine::lauchApplication);

    QObject::connect(&commands, &CommsCommands::exitApplicationRequested,
                     &stateMachine, &UIAppStateMachine::exitApplication);

    QObject::connect(setup.connectionManager, &ConnectionManager::applicationConnectionValidated,
                     &stateMachine, &UIAppStateMachine::applicationConnectionValidated);


    if (params.isSet(CommsParameters::DISABLE_WAITAPP)) {
        waitAppController.enableSimulatedMode(true);
        // as normal launchin is disabled we wire just act of launching for validation
        UIAppStateMachine* pStateMachine = &stateMachine;
        QObject::connect(&waitAppController, &ApplicationController::launched,
                         [=] () { pStateMachine->applicationConnectionValidated("waitapp"); });
        //&stateMachine, &UIAppStateMachine::applicationConnectionValidated);
    }

    if (!params.isSet(CommsParameters::DISABLE_MAINUI)) {
        INFO("Enabling UI statemachine");
        stateMachine.start();
    } else {
        // as normal ui states are disabled (this is development time setup)
        // we set fixed codes so that waitapp and mainui can be identified,
        // they can have specific commands
        setup.applicationRegistry.insertFixedIdentificationCode(
                    waitAppController.application()->meta()->applicationId(),
                    "waitapp");

        setup.applicationRegistry.insertFixedIdentificationCode(
                    mainuiController.application()->meta()->applicationId(),
                    "mainui");
    }

    // quit event loop and tear down nicely when SIGTERM
    //  - especially launched child processes get killed
    auto sighandler = [&] (int s) {
        Q_UNUSED(s);
        qDebug() << "SIGTERM -> Quitting";
        QCoreApplication::quit();
    };

    signal(SIGTERM, sighandler);

    // TODO: pinging gberry server is missing
    INFO("Starting event queue");
    setup.start();
    headServerConnection.open();

    return app.exec();
}

} // eon
