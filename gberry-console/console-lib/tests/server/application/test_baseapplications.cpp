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
 
 #include <testutils/qtgtest.h>

#include "server/application/baseapplications.h"
#include "server/application/baseapplication.h"
#include "server/application/applicationmeta.h"

using namespace GBerry::Console::Server;

QSharedPointer<IApplication> createApplication(
        const QString& applicationId,
        const QString& version,
        bool systemApp = false) {
    ApplicationMeta* meta = new ApplicationMeta();
    meta->setApplicationId(applicationId);
    meta->setVersion(version);
    meta->setIsSystemApp(systemApp);

    BaseApplication* app = new BaseApplication(QSharedPointer<ApplicationMeta>(meta));
    return QSharedPointer<IApplication>(static_cast<IApplication*>(app));
}

TEST(BaseApplications, Applications)
{
    QSharedPointer<IApplication> iapp(createApplication("foobar", "1.0.0"));

    QMap<QString, QSharedPointer<IApplication>> appsMap;
    appsMap[iapp->id()] = iapp;

// -- test applications()
    BaseApplications apps(appsMap);
    ASSERT_EQ(1, apps.applications().length());
    QSharedPointer<IApplication> a1 = apps.applications().at(0);
    ASSERT_FALSE(a1.isNull());
    ASSERT_TRUE(a1->id() == "foobar-1.0.0");

// -- test application()
    QSharedPointer<IApplication> a2 = apps.application("notexists");
    ASSERT_TRUE(a2.isNull());

    a2 = apps.application("foobar-1.0.0");
    ASSERT_FALSE(a2.isNull());

// -- constructor QMap is just source of copy
    appsMap.clear();
    ASSERT_EQ(apps.applications().length(), 1);

// -- adding an app
    QSharedPointer<IApplication> iapp2(createApplication("abc", "1.0.0"));
    apps.add(iapp2);

    ASSERT_EQ(apps.applications().length(), 2);

// -- adding a system app
    QSharedPointer<IApplication> iapp3(createApplication("sys", "1.0.0", true));
    apps.add(iapp3);

    ASSERT_EQ(apps.applications().length(), 2);
    QSharedPointer<IApplication> iapp4 = apps.application("sys-1.0.0");
    ASSERT_FALSE(iapp4.isNull());
}
