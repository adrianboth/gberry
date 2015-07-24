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
 
 #ifndef MOCK_IAPPLICATIONS_H
#define MOCK_IAPPLICATIONS_H

#include "server/application/iapplications.h"

class StubIApplications : public IApplications
{
public:
    StubIApplications(QMap<QString, QSharedPointer<IApplication>>& apps) : _apps(apps) {}

    virtual QList<QSharedPointer<IApplication>> applications() const override { return _apps.values(); }

    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const override { return _apps[uniqueID]; }

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const override {
        QList<QSharedPointer<IApplication>> found;
        foreach (auto app, _apps.values()) {
            if (app->meta()->applicationId() == applicationId)
                found << app;
        }

        return found;
    }

private:
    QMap<QString, QSharedPointer<IApplication>>& _apps;
};

#endif // MOCK_IAPPLICATIONS_H

