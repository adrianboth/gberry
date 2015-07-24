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
 
 //#include "gtest/gtest.h"

#include <QCoreApplication>

#include <testutils/gtestrunner.h>

#define LOG_AREA "tests"
#include "log/log.h"
#include "log/stdoutlogmsghandler.h"
#include "log/logcontrol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    GTestRunner runner(&argc, argv);
    runner.start();
    return a.exec();
}
