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

#include "qtlibrariesmanager.h"

#include <QDir>
#include <QFile>

#include <utils/fileutils.h>

#define LOG_AREA "QtLibrariesManager"
#include "log/log.h"


namespace GBerryComms {

// these are basically all internal errors
// TODO: could be QtLibrariesManagerError category for error??
const QtLibrariesManagerError QtLibrariesManagerErrors::QT_VERSION_NOT_FOUND(500, "QT_VERSION_NOT_FOUND");
const QtLibrariesManagerError QtLibrariesManagerErrors::QT_CONF_WRITING_FAILED(502, "QT_CONF_WRITING_FAILED");

class QtLibrariesManager::Private {
public:
    QString qtVersionsDirPath;
    QString selectedQtVersionDir;
};

QtLibrariesManager::QtLibrariesManager(const QString& qtVersionsDirPath) :
    _d(new QtLibrariesManager::Private)
{
    _d->qtVersionsDirPath = qtVersionsDirPath;
    QDir qtVersionsDir(qtVersionsDirPath);

    QStringList qtDirs = qtVersionsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if (qtDirs.size() == 0) {
        ERROR("Didn't find Qt versions from" << qtVersionsDirPath);
        return;
    }

    if (qtDirs.size() > 1) {
        // better to fail fully than just select one
        WARN("More than one Qt versions. This implementation can't yet select!");
        return;
    }

    // case: only one dir found
    _d->selectedQtVersionDir = qtVersionsDir.filePath(qtDirs.first());
    INFO("Using Qt version" << _d->selectedQtVersionDir << "for applications");
}

QtLibrariesManager::~QtLibrariesManager()
{

}

bool QtLibrariesManager::createQtConf(const QString &dstDirPath, Result &result)
{
    // error if no qtdir selected
    if (_d->selectedQtVersionDir.isEmpty()) {
        result << QtLibrariesManagerErrors::QT_VERSION_NOT_FOUND
               << Result::reasonFromDesc("No Qt versions available. See log for details")
               << Result::Meta("qt_versions_dir", _d->qtVersionsDirPath);
        return false;
    }

    if (!QDir(dstDirPath).exists()) {
        result << QtLibrariesManagerErrors::QT_CONF_WRITING_FAILED
               << Result::reasonFromDesc("Provided destination directory '#{dir_path}' doesn't exist")
               << Result::Meta("dir_path", dstDirPath);
        return false;
    }

    QFile qtconf(GBerryLib::joinpath(dstDirPath, "qt.conf"));
    if (!qtconf.open(QIODevice::WriteOnly)) {
        result << QtLibrariesManagerErrors::QT_CONF_WRITING_FAILED
               << Result::reasonFromDesc("Opening file '#{qtconf}' for writing failed")
               << Result::Meta("qtconf", qtconf.fileName())
               << Result::Meta("qfile_error", qtconf.errorString());
        return false;
    }

    qtconf.write("[Paths]\nPrefix = ");
    qtconf.write(_d->selectedQtVersionDir.toLatin1());

    qtconf.close();

    return true;
}

} // eon
