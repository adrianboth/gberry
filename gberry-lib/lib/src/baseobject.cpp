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
 
 #include "baseobject.h"

#define LOG_AREA "BaseObject"
#include "log/log.h"

namespace GBerryLib {

BaseObject::BaseObject(const QString& className) :
    _objectClassName(className)
{
    static QMap<QString, int> counters;
    if (counters.contains(className)) {
        int currentValue = counters[className];
        currentValue++;
        counters[className] = currentValue;
        _objectId = currentValue;
    } else {
        int currentValue = 0;
        counters[className] = currentValue;
        _objectId = currentValue;
    }

    DEBUG("Constructor:" << _objectClassName << "-" << _objectId);
}

BaseObject::~BaseObject()
{
    TRACE("Destructor:" << _objectClassName << "-" << _objectId);
}

int BaseObject::objectId() const
{
    return _objectId;
}

QString BaseObject::objectClassName() const
{
    return _objectClassName;
}

} // eon
