#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QCommandLineParser>

#include "localapplicationsstorage.h"
#include "localapplications.h"
#include "applicationcontroller.h"
#include "launchcontroller.h"
#include "uiappstatemachine.h"

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>
#include <server/commtcpserver.h>

#include <server/serversetup.h>
#include <server/playersessionmanager.h>

#define LOG_AREA "comms"
#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"


// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("comms");
    QCoreApplication::setApplicationVersion("0.1");

    // --
    QCommandLineParser parser;
    parser.setApplicationDescription("Communication manager of GBerry game platform");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption disableWaitUIOption("disable-wait-ui", "Disables showing waiting application");
    QCommandLineOption disableMainUIOption("disable-main-ui", "Disables showing main application");
    parser.addOption(disableWaitUIOption);
    parser.addOption(disableMainUIOption);

    parser.process(app);

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

    // TODO: read used apps path
    //   - TODO: how to development time path, defined in QtCreator

#ifdef GBERRY_ROOTPATH
    QString gberryRootDirBuildTime(xstr(GBERRY_ROOTPATH));
#else
#error "GBERRY_ROOTPATH not defined"
    // TODO: other alternatives
#endif

    // TODO: command line option and environment variable

    LocalApplicationsStorage appStorage(gberryRootDirBuildTime);// gberryRootDirBuildTime
    LocalApplications apps(&appStorage);

    ApplicationController waitAppController(apps.application("waitapp"));
    ApplicationController mainuiController(apps.application("mainui"));
    LaunchController currentAppController(&apps);

    UIAppStateMachine stateMachine(&waitAppController, &mainuiController, &currentAppController);

    QObject::connect(&setup.controlChannel, &ServerSideControlChannel::applicationLaunchRequested,
                     &stateMachine, &UIAppStateMachine::lauchApplication);

    QObject::connect(&setup.controlChannel, &ServerSideControlChannel::applicationLaunchRequested,
                     &stateMachine, &UIAppStateMachine::exitCurrentApplication);

    QObject::connect(setup.connectionManager, &ConnectionManager::applicationConnectionValidated,
                     &stateMachine, &UIAppStateMachine::applicationConnectionValidated);

    //WaitApplicationController waitapp(appPath);

    if (parser.isSet(disableWaitUIOption)) {
        waitAppController.enableSimulatedMode(true);
        QObject::connect(&waitAppController, &ApplicationController::launched,
                         &stateMachine, &UIAppStateMachine::applicationConnectionValidated);
    }

    if (!parser.isSet(disableMainUIOption)) {
        INFO("Enabling UI statemachine");
        stateMachine.start();
    }

    // TODO: pinging gberry server is missing
    INFO("Starting event queue");

    return app.exec();
}
