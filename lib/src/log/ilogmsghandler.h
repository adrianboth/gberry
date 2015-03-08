#ifndef ILOGMSGHANDLER_H
#define ILOGMSGHANDLER_H

#include <QString>

#include <log/log.h>

class ILogMsgHandler
{
public:
    ILogMsgHandler() : _logLevel(Log::NONE) {}

    int logLevel() const { return _logLevel; }

    virtual void process(const LogMsg& msg) = 0;

protected:
    int _logLevel;
};

Q_DECLARE_INTERFACE(ILogMsgHandler, "geektools.logmsghandler/1.0")

#endif // ILOGMSGHANDLER_H
