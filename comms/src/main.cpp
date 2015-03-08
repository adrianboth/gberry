#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include "gberryconsoleserver.h"

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
    QCoreApplication a(argc, argv);

    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    INFO("Setting up ...");

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;
    setup.start();

    // TODO: pinging gberry server is missing
    INFO("Starting event queue");

    return a.exec();
}
