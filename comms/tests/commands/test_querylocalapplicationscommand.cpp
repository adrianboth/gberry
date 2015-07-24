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
