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

#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QTemporaryDir>

#include "qtlibrariesmanager.h"
using namespace GBerryComms;

#include "utils/fileutils.h"

#define LOG_AREA "QtLibrariesManagerTests"
#include "log/log.h"


TEST(QtLibrariesManager, WriteQtConfOk)
{
    QTemporaryDir tempDir;
    // create fake Qt version dir (=empty)
    QDir versionsDir(tempDir.path());
    versionsDir.mkdir("Qt5.4");
    QString qtDir = GBerryLib::joinpath(tempDir.path(), "Qt5.4");

    QtLibrariesManager qtLibsMgr(tempDir.path());

    Result res;
    ASSERT_TRUE(qtLibsMgr.createQtConf(tempDir.path(), res));

    ASSERT_FALSE(res.hasErrors());
    QString qtconfFilePath = GBerryLib::joinpath(tempDir.path(), "qt.conf");
    ASSERT_TRUE(QFileInfo(qtconfFilePath).exists());

    QFile qtconf(qtconfFilePath);
    qtconf.open(QIODevice::ReadOnly);
    QString data = qtconf.readAll();

    ASSERT_TRUE(data.contains("Prefix = " + qtDir)) << data;

}
