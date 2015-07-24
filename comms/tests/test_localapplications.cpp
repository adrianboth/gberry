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
 
 
#include <gtest/gtest.h>

#include "localapplications.h"

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "mocks/mock_iapplicationstorage.h"
#include "testutils/qtgtest.h"

TEST(LocalApplications, Simple)
{
    QList<QSharedPointer<Application>> appList;
    StubIApplicationsStorage stubStorage(appList);
    LocalApplications apps(&stubStorage);
    ASSERT_TRUE(apps.applications().length() == 0);
    ASSERT_TRUE(apps.applicationsByApplicationId("foobar").length() == 0);
    ASSERT_TRUE(apps.application("notfound").isNull());

    // --
    QSharedPointer<ApplicationMeta> meta1(new ApplicationMeta());
    meta1->setApplicationId("foobar");
    meta1->setVersion("2.0.0");

    QSharedPointer<Application> app1(new Application(meta1));
    appList << app1;

    // not yet found that new apps around
    ASSERT_TRUE(apps.applications().length() == 0);

    stubStorage.emitApplicationUpdated();

    ASSERT_TRUE(apps.applications().length() == 1);
    ASSERT_TRUE(apps.applicationsByApplicationId("foobar").length() == 1);
    ASSERT_TRUE(apps.application("foobar-2.0.0") == app1);

}
