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

#include "cmdlineparams.h"
#include "systemservices.h"
#include "utils/fileutils.h"

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>
#include <server/commtcpserver.h>

#include <server/serversetup.h>
#include <server/playersessionmanager.h>
#include <server/commands/querylocalapplicationscommand.h>

#define LOG_AREA "comms"
#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"


using namespace Comms;
using namespace GBerryLib;


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("comms");
    QCoreApplication::setApplicationVersion("0.1");

    // --
    EnvironmentVariables env(QProcessEnvironment::systemEnvironment());
    CmdLineParams params(env);
    params.parse(app.arguments());

    // --
    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    INFO("Setting up ...");

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;
    setup.start();

    QString rootPath(params.value(CmdLineParams::ROOT_PATH));
    DEBUG("Root path:" << rootPath);

    LocalApplicationsStorage appStorage(joinpath(rootPath, "apps"));
    QSharedPointer<LocalApplications> apps(new LocalApplications(&appStorage));

    QSharedPointer<IApplications> iapps(qSharedPointerCast<IApplications>(apps));
    QSharedPointer<ICommand> cmd(new QueryLocalApplicationsCommand(iapps));
    setup.channelManager.registerCommand(cmd);


    auto getapp = [&] (const QString& appId) {
        QList<QSharedPointer<IApplication>> foundApps = iapps->applicationsByApplicationId(appId);
        // TODO: we good do something nice, during development time without waitapp and mainui is ok
        Q_ASSERT(foundApps.length() > 0);

        // TODO: we should support multiple versions
        if (foundApps.length() > 1)
            WARN("Multiple versions found for application" << appId << ". Selecting first one.");

        return foundApps[0];
    };

    ApplicationController waitAppController(getapp("waitapp"));
    ApplicationController mainuiController(getapp("mainui"));

    // TODO: using safe pointer goes too far ...
    LaunchController currentAppController(apps.data());

    UIAppStateMachine stateMachine(&waitAppController, &mainuiController, &currentAppController);

    QObject::connect(&setup.controlChannel, &ServerSideControlChannel::applicationLaunchRequested,
                     &stateMachine, &UIAppStateMachine::lauchApplication);

    QObject::connect(&setup.controlChannel, &ServerSideControlChannel::applicationLaunchRequested,
                     &stateMachine, &UIAppStateMachine::exitCurrentApplication);

    QObject::connect(setup.connectionManager, &ConnectionManager::applicationConnectionValidated,
                     &stateMachine, &UIAppStateMachine::applicationConnectionValidated);


    if (params.isSet(CmdLineParams::DISABLE_WAITAPP)) {
        waitAppController.enableSimulatedMode(true);
        QObject::connect(&waitAppController, &ApplicationController::launched,
                         &stateMachine, &UIAppStateMachine::applicationConnectionValidated);
    }

    if (!params.isSet(CmdLineParams::DISABLE_MAINUI)) {
        INFO("Enabling UI statemachine");
        stateMachine.start();
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

    return app.exec();
}


