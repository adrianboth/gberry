#ifndef UTIL_ENABLELOG_H
#define UTIL_ENABLELOG_H

#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"

namespace TestUtils
{
// utility class to make call via object creation
class StdoutLoggingEnabler
{
public:
    StdoutLoggingEnabler();
    ~StdoutLoggingEnabler();
};

// enables logging to stdout
StdoutLoggingEnabler& enabledStdoutLogging();

} // eon


// TODO: teardown of logging (in desctructor)

#endif // UTIL_ENABLELOG_H

