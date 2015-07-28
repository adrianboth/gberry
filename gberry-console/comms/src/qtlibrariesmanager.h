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

#ifndef QTLIBRARIESMANAGER_H
#define QTLIBRARIESMANAGER_H

#include <QString>
#include <QScopedPointer>

#include <result.h>
using namespace GBerryLib;

namespace GBerryComms {

ERRORCLASS(QtLibrariesManagerError)

class QtLibrariesManagerErrors {
public:
    static const QtLibrariesManagerError QT_VERSION_NOT_FOUND;
    static const QtLibrariesManagerError QT_CONF_WRITING_FAILED;
};

class QtLibrariesManager
{
public:
    // qtVersionsDir contains versions as subdirectories
    QtLibrariesManager(const QString& qtVersionsDir);
    ~QtLibrariesManager();

    bool createQtConf(const QString& dstDirPath, Result& result);

private:
    class Private;
    const QScopedPointer<Private> _d;
};

} // eon

#endif // QTLIBRARIESMANAGER_H
