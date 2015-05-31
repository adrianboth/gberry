#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QCommandLineParser>

#include "gberryconsoleserver.h"
#include "waitapplicationcontroller.h"
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


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("comms");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Communication manager of GBerry game platform");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption testOption("t", "test option");
    parser.addOption(testOption);

    parser.process(app);

    bool testEnabled = parser.isSet(testOption);


    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    INFO("Setting up ...");

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;
    setup.start();

    LocalApplicationsStorage appStorage;
    LocalApplications apps(&appStorage);

    ApplicationController waitAppController(apps.application("waitapp"));
    ApplicationController mainuiController(apps.application("mainui"));
    LaunchController currentAppController(&apps);

    UIAppStateMachine stateMachine(&waitAppController, &mainuiController, &currentAppController);

    //WaitApplicationController waitapp(appPath);
    if (!testEnabled) {
        INFO("TEST ENABLED");
        //waitapp.launch();
        stateMachine.start();
    }
    // TODO: pinging gberry server is missing
    INFO("Starting event queue");

    return app.exec();
}
