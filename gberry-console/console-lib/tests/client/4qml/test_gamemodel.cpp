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

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include <testutils/waiter.h>

#include "server/application/iapplication.h"
#include "server/application/applicationmeta.h"
#include "server/application/baseapplication.h"
#include "server/application/baseapplications.h"

using namespace GBerry::Console::Server;

#include "common/messagefactory.h"

using namespace GBerry::Console;

#include "client/clientsidecontrolchannel.h"
#include "client/gamemodelcommunication.h"
#include "client/4qml/gamemodel.h"


class TestGameModelCommunication : public IGameModelCommunication
{
public:
    TestGameModelCommunication() : IGameModelCommunication(nullptr) {}
    virtual ~TestGameModelCommunication() {}

    virtual void sendMessage(const QJsonObject &msg) {
        sendMessageCallCount++;
        lastMsg = msg;
    }

    int sendMessageCallCount{0};
    QJsonObject lastMsg;

    void emitMessageReceived(const QJsonObject& msg) { emit messageReceived(msg); }
};

QSharedPointer<IApplication> createApplication(
        const QString& applicationId,
        const QString& version,
        const QString& name,
        const QString& description) {
    ApplicationMeta* meta = new ApplicationMeta();
    meta->setApplicationId(applicationId);
    meta->setVersion(version);
    meta->setName(name);
    meta->setDescription(description);

    BaseApplication* app = new BaseApplication(QSharedPointer<ApplicationMeta>(meta));
    return QSharedPointer<IApplication>(static_cast<IApplication*>(app));
}


TEST(GameModel, RequestLocalApplications)
{
    TestGameModelCommunication comm;
    GameModel model(&comm);

    int localGamesAvailableSignaled{0};
    QObject::connect(&model, &GameModel::localGamesAvailable,
                     [&] () {
        localGamesAvailableSignaled++;
    });

// -- first no games available
    ASSERT_FALSE(model.requestLocalGames());
    ASSERT_TRUE(model.localGameIds().size() == 0);
    ASSERT_TRUE(model.game("fooId-1.1.1").size() == 0);

    ASSERT_TRUE(comm.sendMessageCallCount == 1);

    ASSERT_TRUE(comm.lastMsg.contains("command"));
    ASSERT_TRUE(comm.lastMsg["command"].toString() == "QueryLocalApplications") << comm.lastMsg["command"].toString();

// -- pass a response

    BaseApplications* apps = new BaseApplications;
    QSharedPointer<IApplications> iapps(apps);
    apps->add(createApplication("fooId", "1.1.1", "Foo", "foo desc"));
    QJsonObject responseMsg = MessageFactory::createQueryLocalApplicationsReply(iapps);

    comm.emitMessageReceived(responseMsg);

    WAIT_AND_ASSERT(localGamesAvailableSignaled == 1);
    //Waiter([&] () {return localGamesAvailableSignaled > 0;}).wait(true);
    ASSERT_EQ(localGamesAvailableSignaled, 1);
    ASSERT_TRUE(model.requestLocalGames());
    ASSERT_EQ(model.localGameIds().size(), 1);
    ASSERT_TRUE(model.game("fooId-1.1.1").size() > 0);

    QVariantMap foo = model.game("fooId-1.1.1");
    ASSERT_TRUE(foo["id"].toString() == "fooId-1.1.1");
    ASSERT_TRUE(foo["name"].toString() == "Foo");
    ASSERT_TRUE(foo["description"].toString() == "foo desc");
}


// using real GameModelCommunication
TEST(GameModel, GameModelCommunicationIntegration)
{
    ClientSideControlChannel controlChannel;
    GameModelCommunication comm(&controlChannel);

    GameModel model(&comm);

    int localGamesAvailableSignaled{0};
    QObject::connect(&model, &GameModel::localGamesAvailable,
                     [&] () {
        localGamesAvailableSignaled++;
    });

    // create reply with one app, just something to populate
    BaseApplications* apps = new BaseApplications;
    QSharedPointer<IApplications> iapps(apps);
    apps->add(createApplication("fooId", "1.1.1", "Foo", "foo desc"));

    QJsonObject replyJson = MessageFactory::createQueryLocalApplicationsReply(iapps);
    QJsonDocument jdoc(replyJson);
    QByteArray replyMsg(jdoc.toJson());

// -- test
    controlChannel.receiveMessage(replyMsg);

    WAIT_AND_ASSERT(localGamesAvailableSignaled == 1);
}

// TODO: updated
