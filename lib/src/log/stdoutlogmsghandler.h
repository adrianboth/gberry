#ifndef STDOUTLOGMSGHANDLER_H
#define STDOUTLOGMSGHANDLER_H

#include "ilogmsghandler.h"

class StdoutLogMsgHandler : public ILogMsgHandler
{
public:
    StdoutLogMsgHandler(int logLevel = Log::DEBUG);

    virtual void process(const LogMsg& msg);
};

#endif // STDOUTLOGMSGHANDLER_H
