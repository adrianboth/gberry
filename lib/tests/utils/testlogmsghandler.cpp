#include "testlogmsghandler.h"

TestLogMsgHandler::TestLogMsgHandler(int logLevel)
{
    _logLevel = logLevel;
}

void TestLogMsgHandler::process(const LogMsg &msg)
{
    lastLogMsg = msg;
}
