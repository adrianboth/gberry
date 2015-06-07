
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
