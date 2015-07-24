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
 
 #ifndef ILOGMSGHANDLER_H
#define ILOGMSGHANDLER_H

#include <QString>

#include <log/log.h>

class ILogMsgHandler
{
public:
    explicit ILogMsgHandler() : _logLevel(Log::NONE) {}
    virtual ~ILogMsgHandler() {}

    int logLevel() const { return _logLevel; }

    virtual void process(const LogMsg& msg) = 0;

protected:
    int _logLevel;
};

Q_DECLARE_INTERFACE(ILogMsgHandler, "geektools.logmsghandler/1.0")

#endif // ILOGMSGHANDLER_H
