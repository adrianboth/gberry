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

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;

    QString rootPath(params.value(CommsParameters::ROOT_PATH));
    DEBUG("Root path:" << rootPath);

    LocalApplicationsStorage appStorage(joinpath(rootPath, "apps"));
    QSharedPointer<LocalApplications> apps(new LocalApplications(&appStorage));

    QSharedPointer<IApplications> iapps(qSharedPointerCast<IApplications>(apps));

    InvocationFactoryImpl invocationFactory;
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

    DEBUG("Initializing configuration for 'waitapp' and 'mainui'");
    ApplicationController waitAppController(getapp("waitapp"), &setup.applicationRegistry);
    ApplicationController mainuiController(getapp("mainui"), &setup.applicationRegistry);

    // TODO: using safe pointer goes too far ...
    LaunchController currentAppController(apps.data());

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
    return app.exec();
}

} // eon
