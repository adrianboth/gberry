//#include "gtest/gtest.h"

#include <QCoreApplication>

#include <testutils/gtestrunner.h>

#define LOG_AREA "tests"
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

    GTestRunner runner(&argc, argv);
    runner.start();
    return a.exec();
}
