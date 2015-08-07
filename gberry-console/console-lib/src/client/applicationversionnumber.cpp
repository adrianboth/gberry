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

#include "applicationversionnumber.h"

#include <QStringList>

namespace GBerryLib {

ApplicationVersionNumber::ApplicationVersionNumber()
{

}

ApplicationVersionNumber::~ApplicationVersionNumber()
{

}

// static
bool ApplicationVersionNumber::versionIsGreater(const QString& v1, const QString& v2)
{
    QStringList v1Parts = v1.split('.');
    QStringList v2Parts = v2.split('.');

    int c = 0;
    while (true) {
        if (v1Parts.length() < c+1 || v2Parts.length() < c+1) {
            // end situation
            // missing part is assumed to be zero
            if (v1Parts.length() > v2Parts.length() && v1Parts.at(c) != "0") {
                return true;
            }
            if (v2Parts.length() > v1Parts.length() && v2Parts.at(c) != "0") {
                return false;
            }
            // version string as equal, size and we did run out
            //  --> not creater
            return false;
        }

        int i1 = v1Parts.at(c).toInt();
        int i2 = v2Parts.at(c).toInt();
        if (i1 > i2)
            return true;
        if (i2 > i1)
            return false;

        // equal -> continue to next number
        c++;
    }
}

} // eon
