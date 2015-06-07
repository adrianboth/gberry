#include <testutils/qtgtest.h>

#include "server/application/application2json.h"
#include "server/application/baseapplication.h"

using namespace GBerry::Console::Server;

TEST(Application2Json, BasicConversion)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta);
    meta->setApplicationId("foo");
    meta->setVersion("1.2.3");
    meta->setName("FooApp");
    meta->setDescription(("foodesc"));
    meta->setCatalogImageFilePath("foobar.png");

    BaseApplication app(meta);

    QJsonObject json = Application2Json::from(app);

    EXPECT_TRUE(json["id"] == "foo-1.2.3");
    EXPECT_TRUE(json["application_id"] == "foo");
    EXPECT_TRUE(json["version"] == "1.2.3");
    EXPECT_TRUE(json["description"] == "foodesc");
    EXPECT_TRUE(json["name"] == "FooApp");
    EXPECT_TRUE(json["catalog_image"] == "foobar.png");

}
