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

// TODO: updated
