#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonDocument>
#include <QJsonDocument>

#include "common/messagefactory.h"

using namespace GBerry::Console;

#include "server/application/baseapplications.h"
using namespace GBerry::Console::Server;

#include "client/gamemodelcommunication.h"
#include "client/clientsidecontrolchannel.h"


TEST(GameModelCommunication, BasicUseCase)
{
    ClientSideControlChannel controlChannel;
    GameModelCommunication comm(&controlChannel);

    // verify that gamemodel communication works as expected because now for
    // some reason signals from parsing a reply are not passing

    bool messageReceivedSignal{false};
    QJsonObject receivedJson;
    QObject::connect(&comm, &IGameModelCommunication::messageReceived,
                     [&] (const QJsonObject& msg) {
        messageReceivedSignal = true;
        receivedJson = msg;
    });

    // no real apps, just empty list, testing signal passing
    BaseApplications* apps = new BaseApplications;
    QSharedPointer<IApplications> iapps(apps);

    QJsonObject replyJson = MessageFactory::createQueryLocalApplicationsReply(iapps);
    QJsonDocument jdoc(replyJson);
    QByteArray replyMsg(jdoc.toJson());

// -- test
    controlChannel.receiveMessage(replyMsg);

    WAIT_AND_ASSERT(messageReceivedSignal);
    EXPECT_TRUE(receivedJson == replyJson);
}
