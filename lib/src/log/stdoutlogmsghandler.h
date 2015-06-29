#ifndef STDOUTLOGMSGHANDLER_H
#define STDOUTLOGMSGHANDLER_H

#include "ilogmsghandler.h"

class StdoutLogMsgHandler : public ILogMsgHandler
{
public:
    explicit StdoutLogMsgHandler(int logLevel = Log::DEBUG);
    virtual ~StdoutLogMsgHandler();

    virtual void process(const LogMsg& msg) override;
};

#endif // STDOUTLOGMSGHANDLER_H
