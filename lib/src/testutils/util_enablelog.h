#ifndef UTIL_ENABLELOG_H
#define UTIL_ENABLELOG_H

#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"

namespace TestUtils
{
void enabledStdoutLogging()
{
    static bool enabled = false;
    if (!enabled)
    {
        StdoutLogMsgHandler* handler = new StdoutLogMsgHandler(Log::TRACE);
        LogControl* logControl = new LogControl();
        logControl->registerMsgHandler(handler);
        Log::singleton().use(logControl);
        enabled = true;
    }
}
} // eon

// TODO: teardown of logging

#endif // UTIL_ENABLELOG_H

