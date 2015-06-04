#include "util_enablelog.h"

namespace TestUtils
{
StdoutLoggingEnabler& enabledStdoutLogging()
{
    static StdoutLoggingEnabler enabler; // not thread safe
    return enabler;
}

StdoutLoggingEnabler::StdoutLoggingEnabler()
{
    StdoutLogMsgHandler* handler = new StdoutLogMsgHandler(Log::TRACE);
    LogControl* logControl = new LogControl();
    logControl->registerMsgHandler(handler);
    Log::singleton().use(logControl);
}

StdoutLoggingEnabler::~StdoutLoggingEnabler()
{

}
} // eon
