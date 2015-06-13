
#include <testutils/qtgtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include <testutils/waiter.h>

#include "server/connectionmanager.h"
#include "server/channelfactory.h"
#include "messagefactory.h"

#include "utils/testchannel.h"
#include "utils/testchannelfactory.h"
#include "utils/testplayerchannelnorthpartner.h"
#include "utils/util_messageparser.h"

#define LOG_AREA "test_connectionmanager"
#include "log/log.h"

using namespace GBerry::Console;
using namespace GBerry::Console::Server;

TEST(ConnectionManager, ValidateConnection)
{
    //ConnectionManager manager;
}
