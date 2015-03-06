#ifndef CONSOLERESTSERVER_H
#define CONSOLERESTSERVER_H

#include <QObject>
#include "server/playersessionmanager.h"

class QHttpServer;
class QHttpRequest;
class QHttpResponse;

class ConsoleRESTServer : public QObject
{
    Q_OBJECT

public:
    ConsoleRESTServer(PlayerSessionManager& sessionManager);
    ~ConsoleRESTServer();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    // expects all body data to be available
    void handlePostRequest(QHttpRequest *req, QHttpResponse *resp);

    QHttpServer * _server;
    PlayerSessionManager& _sessionManager;
    int _tokenCounter;
};

#endif // CONSOLERESTSERVER_H
