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
 
 #ifndef BASEAPPLICATIONS_H
#define BASEAPPLICATIONS_H

#include <QMap>

#include "server/application/iapplications.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class BaseApplications : public IApplications
{
public:
    explicit BaseApplications();
    explicit BaseApplications(QMap<QString, QSharedPointer<IApplication>>& apps);
    virtual ~BaseApplications();

    // -- IApplications
    virtual QList<QSharedPointer<IApplication>> applications() const override;
    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const override;
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const override;

    void add(QSharedPointer<IApplication> app);

protected:
    QMap<QString, QSharedPointer<IApplication>> _apps;

};

}}} // eon

#endif // BASEAPPLICATIONS_H
