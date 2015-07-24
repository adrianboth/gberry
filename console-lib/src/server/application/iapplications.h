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
 
 #ifndef IAPPLICATIONS_H
#define IAPPLICATIONS_H

#include <QSharedPointer>
#include <QString>
#include <QList>

#include <baseobject.h>
#include "iapplication.h"

#include <QDebug>

namespace GBerry {
    namespace Console {
        namespace Server {

class IApplications : public GBerryLib::BaseObject
{
public:
    explicit IApplications() : GBerryLib::BaseObject("IApplications") {}
    virtual ~IApplications() {}

public:
    // all applications but systemapps excluded
    virtual QList<QSharedPointer<IApplication>> applications() const = 0;

    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const = 0;

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const = 0;
};

}}} // eon

#endif // IAPPLICATIONS_H
