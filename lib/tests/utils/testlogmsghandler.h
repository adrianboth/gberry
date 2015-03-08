#ifndef TESTLOGMSGHANDLER_H
#define TESTLOGMSGHANDLER_H

#include <QString>

#include <ilogmsghandler.h>

class TestLogMsgHandler : public ILogMsgHandler
{
public:
    TestLogMsgHandler();

    QString getLatestMsg() const { return lastLogMsg.text; }

    // from LogMsgHandler
    void process(const LogMsg& msg);

    LogMsg lastLogMsg;
};

#endif // TESTLOGMSGHANDLER_H
