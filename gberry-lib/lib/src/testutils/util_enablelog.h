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
 
 /**
 * This header file provides utilities to easily enable logging when running
 * unit tests.
 *
 * Usage:
 * #include "testutils/util_enablelog.h"
 * namespace {
 *     static TestUtils::StdoutLoggingEnabler& logging = TestUtils::enabledStdoutLogging();
 * }
 *
 */

#ifndef UTIL_ENABLELOG_H
#define UTIL_ENABLELOG_H

#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"

#define ENABLE_LOGGING() namespace { static TestUtils::StdoutLoggingEnabler& logging = TestUtils::enabledStdoutLogging(); }

namespace TestUtils
{
// utility class to make call via object creation
class StdoutLoggingEnabler
{
public:
    StdoutLoggingEnabler();
    ~StdoutLoggingEnabler();
};

// enables logging to stdout
StdoutLoggingEnabler& enabledStdoutLogging();

} // eon


// TODO: teardown of logging (in desctructor)

#endif // UTIL_ENABLELOG_H

