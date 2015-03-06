#include <gtest/gtest.h>

#include "testutils/waiter.h"

#include "server/playersessionmanager.h"
#include "server/consolerestserver.h"
#include "server/websocketserver.h"
#include "client/consolesessionmanager.h"
#include "client/consoledevice.h"

using namespace mobile;


TEST(RESTAPI, ConsoleRESTServerAndConsoleSessionManagerIntegration)
{
    PlayerSessionManager serverSessionManager;
    ConsoleRESTServer restServer(serverSessionManager);
    WebsocketServer websocketServer(&serverSessionManager);
    websocketServer.start();

    ConsoleSessionManager clientSessionManager;

    bool clientSessionIsOpen = false;
    QObject::connect(&clientSessionManager, &ConsoleSessionManager::consoleSessionOpened,
                     [&] () { clientSessionIsOpen = true; });

    // --
    ConsoleDevice console("localhost");
    clientSessionManager.open(console, "BarGuest");

    WAIT_CUSTOM_AND_ASSERT(clientSessionIsOpen, 5000, 50);
}

