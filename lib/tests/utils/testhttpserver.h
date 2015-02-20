#ifndef TESTHTTPSERVER_H
#define TESTHTTPSERVER_H

#include <QObject>

#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"


class TestHttpServer : public QObject
{
    Q_OBJECT

public:
    explicit TestHttpServer(uint port = 8080, QObject *parent = 0);
    ~TestHttpServer();

    bool isRequestReceived() const;
    QString lastMessageString() const { return _msg; }

    void resetState();

signals:
    // new request arrived
    void request(QHttpRequest *req, QHttpResponse *resp);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
    void finishRequest();

private:
    uint _port;
    QString _path;
    QString _msg;
    bool _requestReceived;
    QHttpRequest* _currentReq;
    QHttpServer* _server;
};

#endif // TESTHTTPSERVER_H
