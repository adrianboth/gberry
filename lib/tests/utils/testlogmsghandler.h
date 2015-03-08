#ifndef TESTLOGMSGHANDLER_H
#define TESTLOGMSGHANDLER_H

#include <QString>

#include <log/ilogmsghandler.h>

class TestLogMsgHandler : public ILogMsgHandler
{
public:
    TestLogMsgHandler(int logLevel = Log::TRACE);

    QString getLatestMsg() const { return lastLogMsg.text; }

    // from LogMsgHandler
    void process(const LogMsg& msg);

    LogMsg lastLogMsg;
};

#endif // TESTLOGMSGHANDLER_H
