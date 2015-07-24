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
 
 #include "logcontrol.h"

#include <QList>
#include <QDebug>

#include "ilogmsghandler.h"

class LogControl::PrivateLogControlImpl
{
public:
    QList<ILogMsgHandler*> handlers;
};

LogControl::LogControl() :
    _impl(new PrivateLogControlImpl()),
    _highestLogLevel(Log::NONE)
{
}

LogControl::~LogControl()
{
    delete _impl;
    // TODO: delete also handlers, document
}

void LogControl::registerMsgHandler(ILogMsgHandler* msgHandler)
{
    _impl->handlers.append(msgHandler);
    int logLevel = msgHandler->logLevel();
    if (logLevel > _highestLogLevel)
    {
        _highestLogLevel = logLevel;
        emit highestLogLevel();
    }
}

QList<ILogMsgHandler*> LogControl::handlers(const QString &msg) const
{
    Q_UNUSED(msg)
    // TODO: impl handler filtering, now return all
    return _impl->handlers;
}
