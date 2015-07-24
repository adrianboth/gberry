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
 
 #ifndef UTIL_TESTDATA_H
#define UTIL_TESTDATA_H

#include <QDir>

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

namespace TestUtils
{
QString testdataDirPath(QString caseName)
{
    QDir testdataRoot = QDir(xstr(TESTDATA_ROOT));
    return testdataRoot.filePath(caseName);
}
} // eon
#endif // UTIL_TESTDATA_H

