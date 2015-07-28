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

#include "application.h"
#include "server/applicationregistry.h"
#include "applicationexecutionsetup.h"
#include "qtlibrariesmanager.h"

using namespace GBerryComms;

#include "utils/fileutils.h"

#define LOG_AREA "ApplicationExecutionSetupTests"
#include "log/log.h"


TEST(ApplicationExecutionSetup, OkSetup)
{
    QTemporaryDir qtTempDir;
    // create fake Qt version dir (=empty)
    QDir versionsDir(qtTempDir.path());
    versionsDir.mkdir("Qt5.4");

    QtLibrariesManager qtLibsMgr(qtTempDir.path());

    // --
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta()); // this now linux only
    meta->setName("testApp");

    QTemporaryDir tempAppDir; // dir created right away
    QFile myexeFile(tempAppDir.path() + "/testapp"); // setup verifies whether binary can be found
    myexeFile.open(QIODevice::WriteOnly);
    myexeFile.write("!#/bin/bash\nsleep 60");
    myexeFile.flush();
    myexeFile.close();
    myexeFile.setPermissions(QFileDevice::ExeOwner | QFileDevice::WriteOwner | QFileDevice::ReadUser);

    meta->setApplicationExecutablePath(myexeFile.fileName());
    meta->setApplicationDirPath(tempAppDir.path());

    QSharedPointer<Application> app(new Application(meta));
    QSharedPointer<IApplication> iapp(qSharedPointerCast<IApplication>(app));

    // --
    ApplicationRegistry registry;

// -- test

    ApplicationExecutionSetup setup(&qtLibsMgr, &registry, true);

    QProcess process;
    Result res;
    bool ok = setup.prepare(process, *iapp.data(), res);

    ASSERT_TRUE(ok);
    ASSERT_FALSE(res.hasErrors());

    QString qtconfFilePath = GBerryLib::joinpath(tempAppDir.path(), "qt.conf");
    ASSERT_TRUE(QFileInfo(qtconfFilePath).exists());

    QString runshFilePath = GBerryLib::joinpath(tempAppDir.path(), "run.sh");
    ASSERT_TRUE(QFileInfo(runshFilePath).exists());
    ASSERT_TRUE(process.program() == runshFilePath);

    ASSERT_TRUE(process.arguments().size() > 0);

    // TODO: verify output logging channel
    QString logFilePath = GBerryLib::joinpath(tempAppDir.path(), "output.log");
    ASSERT_TRUE(QFileInfo(logFilePath).exists());

}
