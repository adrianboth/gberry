#ifndef GBERRYCONSOLESERVER_H
#define GBERRYCONSOLESERVER_H

#include <QObject>

class QHttpServer;
class QHttpRequest;
class QHttpResponse;

class GBerryConsoleServer : public QObject
{
    Q_OBJECT

public:
    GBerryConsoleServer();
    ~GBerryConsoleServer();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    QHttpServer * _server;
};

#endif // GBERRYCONSOLESERVER_H
