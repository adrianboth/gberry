#ifndef CONSOLERESTSERVER_H
#define CONSOLERESTSERVER_H

#include <QObject>

class QHttpServer;
class QHttpRequest;
class QHttpResponse;

class ConsoleRESTServer : public QObject
{
    Q_OBJECT

public:
    ConsoleRESTServer();
    ~ConsoleRESTServer();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    // expects all body data to be available
    void handlePostRequest(QHttpRequest *req, QHttpResponse *resp);

    QHttpServer * _server;
};

#endif // CONSOLERESTSERVER_H
