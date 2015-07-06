#ifndef CONSOLESESSIONMANAGER_H
#define CONSOLESESSIONMANAGER_H

#include <QObject>
#include <QString>

#include "consoledevice.h"
#include "websocketclient.h"
#include "restinvocation.h"

class InvocationFactory;


namespace mobile
{

class ConsoleSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleSessionManager(QObject *parent = 0);
    ~ConsoleSessionManager();

    void open(ConsoleDevice console, QString playerName);
    void openWebsocket(QString token);
    void close();

    WebsocketClient& websocket() { return _websocket; }

    bool isOpen() const { return _websocket.isConnected(); }

signals:
    void consoleSessionOpenFailed(QString errorMessage);
    void consoleSessionOpened();

public slots:
    void onOpenConsoleSessionFinished(Invocation* invocation);
    void onOpenConsoleSessionError(Invocation* invocation);
    void onWebsocketConnected();

private:
    void initRESTInvocationFactory(ConsoleDevice console);
    QString _hostAddress;
    WebsocketClient _websocket;
    InvocationFactory* _restInvocationFactory;
};

} // eon

#endif // CONSOLESESSIONMANAGER_H
