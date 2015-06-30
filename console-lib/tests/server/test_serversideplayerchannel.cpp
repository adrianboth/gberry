#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonObject>
#include <QJsonDocument>

#include "server/serversideplayerchannel.h"
#include "common/playermeta.h"


TEST(ServerSidePlayerChannel, GoToMainMenu)
{
    const int channelId = 111; // just random number
    const PlayerMeta pmeta(100, "foobar");

    ServerSidePlayerChannel channel(channelId, pmeta);

    int mainMenuRequestedSignalCount{0};
    QObject::connect(&channel, &ServerSidePlayerChannel::playerRequestedMainMenu,
                     [&] () { mainMenuRequestedSignalCount++; });

    QJsonObject json;
    json["action"] = "GoToMainMenu";
    QByteArray msg(QJsonDocument(json).toJson());

// -- test
    channel.receivePlayerMessageFromNorth(msg);

    WAIT_AND_ASSERT(mainMenuRequestedSignalCount == 1);

}

