#include <testutils/qtgtest.h>

#include <QJsonArray>

#include "commands/querylocalapplicationscommand.h"
#include "server/application/baseapplication.h"
#include "server/application/baseapplications.h"

using namespace GBerry::Console::Server;
using namespace GBerryComms;

class TestCommandResponse : public ICommandResponse
{
public:
    void set(QJsonObject json_) override { json = json_; }
    QJsonObject json;
};


TEST(QueryLocalApplicationsCommand, BasicQuery)
{
    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta);
    meta->setApplicationId("foo");
    meta->setVersion("1.2.3");
    meta->setName("FooApp");
    meta->setDescription(("foodesc"));
    meta->setCatalogImageFilePath("foobar.png");

    QSharedPointer<BaseApplication> app(new BaseApplication(meta));
    QSharedPointer<IApplication> iapp(qSharedPointerCast<IApplication>(app));

    QSharedPointer<BaseApplications> apps(new BaseApplications);
    QSharedPointer<IApplications> iapps(qSharedPointerCast<IApplications>(apps));
    apps->add(iapp);

    QueryLocalApplicationsCommand cmd(iapps);

    QJsonObject incomingJson;
    incomingJson["command"] = "QueryLocalApplications";

    TestCommandResponse response;

    bool result = cmd.process(incomingJson, response);
    EXPECT_TRUE(result);
    QJsonArray arr = response.json["applications"].toArray();
    ASSERT_EQ(arr.count(), 1);
    ASSERT_TRUE(arr.at(0).isObject());
    QJsonObject appJson = arr.at(0).toObject();
    EXPECT_TRUE(appJson["id"] == "foo-1.2.3") << response.json["id"].toString();
}
