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
 
 #ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QString>

namespace GBerryLib {

/**
 * This is utility class to track objects, their creating, desctruction and usage.
 */
class BaseObject
{
public:
    explicit BaseObject(const QString& className = QString("default"));
    virtual ~BaseObject();

    int objectId() const;
    QString objectClassName() const;

private:
    int _objectId{-1};
    QString _objectClassName;

};

} // eon

#endif // BASEOBJECT_H
