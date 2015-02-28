#include <gtest/gtest.h>

#include "server/websocketserver.h"
#include "client/websocketclient.h"

#include "testutils/waiter.h"

TEST(WebSockets, ConnectDisconnect)
{
    WebsocketServer server(8888);
    // TODO: now starts automatically

    // TODO: where url part is visible?
    QUrl url(QString("ws://localhost:8888"));
    WebsocketClient client(url);

    // TODO: at this point server is echo server

    QString clientReceivedMessage;
    QObject::connect(&client, &WebsocketClient::messageReceived,
                     [&] (QString message) { clientReceivedMessage = message; });

    Waiter::waitAndAssert([&] () { return client.isConnected(); });
    Waiter::waitAndAssert([&] () { return clientReceivedMessage.length() > 0; });

    // -- second message
    clientReceivedMessage.clear();
    client.sendMessage(("FOOBAR message"));

    Waiter::waitAndAssert([&] () { return clientReceivedMessage.length() > 0; });
}

