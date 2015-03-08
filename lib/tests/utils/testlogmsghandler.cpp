#include "testlogmsghandler.h"

TestLogMsgHandler::TestLogMsgHandler()
{
}

void TestLogMsgHandler::process(const LogMsg &msg)
{
    lastLogMsg = msg;
}
