#include "stdoutlogmsghandler.h"

#include <QTextStream>
#include <QStringList>
#include <QDateTime>

StdoutLogMsgHandler::StdoutLogMsgHandler(int logLevel)
{
    _logLevel = logLevel;
}

void StdoutLogMsgHandler::process(const LogMsg& msg)
{
    // fixed formatting
    // <time> D <area> <file>:<line> : <text>
    QStringList buffer;
    QDateTime datetime = QDateTime::fromMSecsSinceEpoch(msg.time);

    buffer << datetime.toString("dd.MM.yyyy hh:mm:ss.zzz") << " ";
    switch (msg.level)
    {
        case Log::TRACE:
            buffer << "T ";
            break;
        case Log::DEBUG:
            buffer << "D ";
            break;
        case Log::INFO:
            buffer << "I ";
            break;
        case Log::WARN:
            buffer << "W ";
            break;
        case Log::ERROR:
            buffer << "E ";
            break;
    }

    buffer << msg.area << " ";
    int indexOfLastSlash = msg.sourceFile.lastIndexOf("/");
    if (indexOfLastSlash > 0)
    {
        buffer << msg.sourceFile.right(msg.sourceFile.length() - indexOfLastSlash - 1);
    }
    else
    {
        buffer << msg.sourceFile;
    }

    if (msg.sourceLine > 0)
    {
        buffer << ":" << QString::number(msg.sourceLine);
    }

    buffer << " - " << msg.text << "\n";
    QTextStream(stdout) << buffer.join("");
}
