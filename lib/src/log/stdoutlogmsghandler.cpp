/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "stdoutlogmsghandler.h"

#include <QTextStream>
#include <QStringList>
#include <QDateTime>

StdoutLogMsgHandler::StdoutLogMsgHandler(int logLevel)
{
    _logLevel = logLevel;
}

StdoutLogMsgHandler::~StdoutLogMsgHandler()
{
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
