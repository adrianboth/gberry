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
 
 #include "baseapplications.h"


namespace GBerry {
    namespace Console {
        namespace Server {

BaseApplications::BaseApplications()
{

}

BaseApplications::BaseApplications(QMap<QString, QSharedPointer<IApplication>>& apps) :
    _apps(apps)
{
}

BaseApplications::~BaseApplications()
{
}


QList<QSharedPointer<IApplication>> BaseApplications::applications() const
{
    QList<QSharedPointer<IApplication>> found;
    foreach (auto app, _apps.values()) {
        if (!app->meta()->isSystemApp())
            found << app;
    }
    return found;
}

QSharedPointer<IApplication> BaseApplications::application(const QString& uniqueID) const
{
    if (_apps.contains(uniqueID))
        return _apps[uniqueID];
    else
        return QSharedPointer<IApplication>(nullptr);
}

// there can be multiple versions for single application
QList<QSharedPointer<IApplication>> BaseApplications::applicationsByApplicationId(const QString& applicationId) const
{
    QList<QSharedPointer<IApplication>> found;
    foreach (auto app, _apps.values()) {
        if (app->meta()->applicationId() == applicationId)
            found << app;
    }

    return found;
}

void BaseApplications::add(QSharedPointer<IApplication> app)
{
    _apps[app->id()] = app;
}

}}} // eon
