#include <gtest/gtest.h>
#include "testutils/qtgtest.h"

#include <QDateTime>

#define LOG_AREA "TestLogArea"
#include "log/log.h"
#include "log/logcontrol.h"
#include "log/stdoutlogmsghandler.h"

#include "utils/testlogmsghandler.h"

static const char* DEFAULT_LOGAREA = "default";
static const char* DEFAULT_SRC_FILE = "source file";
static const int DEFAULT_LINENO = 10;

TEST(Log, BasicLoggingUsingDirectLoggingFunctions)
{
    TestLogMsgHandler handler;
    LogControl logControl;
    logControl.registerMsgHandler(&handler);

    Log log;
    log.use(&logControl);
    log.begin(Log::TRACE, DEFAULT_LOGAREA, DEFAULT_SRC_FILE, DEFAULT_LINENO);
    log << "trace message";
    log << Log::eol;

    EXPECT_TRUE(handler.getLatestMsg() == QString("trace message"));

    // second message
    log.begin(Log::TRACE, DEFAULT_LOGAREA, DEFAULT_SRC_FILE, DEFAULT_LINENO) << "second msg" << "works" << Log::eol;
    EXPECT_TRUE(handler.getLatestMsg() == QString("second msg works"));
}

TEST(Log, Trace)
{
    StdoutLogMsgHandler handler;
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    TRACE("trace message");
    TRACE_FUNC();
    TRACE_FUNC2("trace func message");
    TRACE_FUNC_IN();
    TRACE_FUNC_OUT();
}

TEST(Log, MacroUsageWithCustomLog)
{
    TestLogMsgHandler handler;
    LogControl logControl;
    logControl.registerMsgHandler(&handler);

    Log log;
    log.use(&logControl);

    // using trace as example
    LOG_TRACE(log, "trace message");
    EXPECT_TRUE(handler.getLatestMsg() == QString("trace message"));

    // second message
    LOG_TRACE(log, "second msg" << "works");
    EXPECT_TRUE(handler.getLatestMsg() == QString("second msg works"));
}

TEST(Log, Singleton)
{
    qint64 t1 = QDateTime::currentMSecsSinceEpoch();
    TestLogMsgHandler handler;
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    TRACE("trace message");
    LogMsg got = handler.lastLogMsg;
    EXPECT_TRUE(got.text == QString("trace message"));
    EXPECT_TRUE(got.area == QString("TestLogArea")); // log area of this file
    // current source file (remember to fix if file named!)
    EXPECT_TRUE(got.sourceFile.endsWith("test_log.cpp")) << "Got:" << got.sourceFile;
    EXPECT_TRUE(got.level == Log::TRACE);
    qDebug() << "t1:" << t1 << ", log:" << got.time;
    EXPECT_TRUE(got.time - t1  >= 0);
    EXPECT_TRUE(got.sourceLine > 10);

    Log::singleton().reset();
}

TEST(Log, DifferentLogLevels)
{
    TestLogMsgHandler handler;
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    TRACE("trace message");
    EXPECT_TRUE(handler.lastLogMsg.text == QString("trace message"));
    EXPECT_TRUE(handler.lastLogMsg.level == Log::TRACE);

    DEBUG("debug message");
    EXPECT_TRUE(handler.lastLogMsg.text == QString("debug message"));
    EXPECT_TRUE(handler.lastLogMsg.level == Log::DEBUG);

    INFO("info message");
    EXPECT_TRUE(handler.lastLogMsg.text == QString("info message"));
    EXPECT_TRUE(handler.lastLogMsg.level == Log::INFO);

    WARN("warning message");
    EXPECT_TRUE(handler.lastLogMsg.text == QString("warning message"));
    EXPECT_TRUE(handler.lastLogMsg.level == Log::WARN);

    ERROR("error message");
    EXPECT_TRUE(handler.lastLogMsg.text == QString("error message"));
    EXPECT_TRUE(handler.lastLogMsg.level == Log::ERROR);

    TRACE("trace with params" << 1 << 2);
    EXPECT_TRUE(handler.lastLogMsg.text == QString("trace with params 1 2"));
    DEBUG("debug with params" << 1 << 2);
    EXPECT_TRUE(handler.lastLogMsg.text == QString("debug with params 1 2"));
    INFO("info with params" << 1 << 2);
    EXPECT_TRUE(handler.lastLogMsg.text == QString("info with params 1 2"));
    WARN("warn with params" << 1 << 2);
    EXPECT_TRUE(handler.lastLogMsg.text == QString("warn with params 1 2"));
    ERROR("error with params" << 1 << 2);
    EXPECT_TRUE(handler.lastLogMsg.text == QString("error with params 1 2"));

    Log::singleton().reset();
}

TEST(Log, StdoutLogMsgHandler)
{
    LogControl logControl;
    EXPECT_TRUE(logControl.highestLogLevel() == Log::NONE);

    StdoutLogMsgHandler handler;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    EXPECT_TRUE(logControl.highestLogLevel() == Log::DEBUG);

    // --
    TRACE("trace message" << "OK");
    DEBUG("debug message");
    INFO("info message");

    StdoutLogMsgHandler handler2(Log::TRACE);
    logControl.registerMsgHandler(&handler2);
    EXPECT_TRUE(logControl.highestLogLevel() == Log::TRACE);

}

TEST(Log, MultipleLogMsgHandlers)
{
    TestLogMsgHandler handler1;
    TestLogMsgHandler handler2;
    LogControl logControl;
    logControl.registerMsgHandler(&handler1);
    logControl.registerMsgHandler(&handler2);
    Log::singleton().use(&logControl);

    TRACE("message1");
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("message1"));
    EXPECT_TRUE(handler2.lastLogMsg.text == QString("message1"));
}

/* How to run and record perf with GTest
TEST(Log, Performance)
{
    TestLogMsgHandler handler1;
    TestLogMsgHandler handler2;
    LogControl logControl;
    logControl.registerMsgHandler(&handler1);
    logControl.registerMsgHandler(&handler2);
    Log::singleton().use(&logControl);

    QBENCHMARK {
        TRACE("message1");
    }
}
*/

TEST(Log, Types)
{
    TestLogMsgHandler handler1;
    LogControl logControl;
    logControl.registerMsgHandler(&handler1);
    Log::singleton().use(&logControl);

    char c = 'c';
    TRACE("msg:" << QChar('a') << 'b' << c);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: a b c"));

    TRACE("msg:" << false << true);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: false true"));

    signed short signedShort = 10;
    unsigned short unsignedShort = 20;
    TRACE("msg:" << signedShort << unsignedShort);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: 10 20"));

    signed int signedInt = 10;
    unsigned int unsignedInt = 20;
    TRACE("msg:" << signedInt << unsignedInt);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: 10 20"));

    signed long signedLong = 10;
    unsigned long unsignedLong = 20;
    TRACE("msg:" << signedLong << unsignedLong);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: 10 20"));

    qint64 signedLong64 = 10;
    quint64 unsignedLong64 = 20;
    TRACE("msg:" << signedLong64 << unsignedLong64);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: 10 20"));

    float f = 0.5;
    double d = 1.55;
    TRACE("msg:" << f << d);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: 0.5 1.55"));

    QString str("str1");
    TRACE("msg:" << QStringRef(&str, 1, 2));
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: tr"));

    TRACE("msg:" << QLatin1String("str1"));
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: str1"));

    QByteArray bytes("string1", 3);
    TRACE("msg:" << bytes);
    EXPECT_TRUE(handler1.lastLogMsg.text == QString("msg: str"));

}
