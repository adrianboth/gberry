#include "testhttpserver.h"

#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"


TestHttpServer::TestHttpServer(uint port, QObject *parent) :
    QObject(parent),
    _requestReceived(false),
    _currentReq(nullptr)
{
    _server = new QHttpServer(this);

    connect(_server, &QHttpServer::newRequest,
            this,   &TestHttpServer::handleRequest);

    _server->listen(QHostAddress::LocalHost, port);

}

TestHttpServer::~TestHttpServer()
{
    _server->close();
}

bool TestHttpServer::isRequestReceived() const
{
    return _requestReceived;
}

void TestHttpServer::resetState()
{
    _requestReceived = false;
    _msg.clear();
}

void TestHttpServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    //QByteArray body = "OK";
    //resp->setHeader("Content-Length", QString::number(body.size()));
    //resp->writeHead(200);
    //resp->end(body);

    connect(req, &QHttpRequest::end,
            this, &TestHttpServer::finishRequest);

    // wait all possible body data to be available
    _currentReq = req;

    // TODO: handle this later
    req->storeBody();

    // TODO: memory management ???
    emit request(req, resp);
    qDebug("### EMITTED request()");
}

void TestHttpServer::finishRequest()
{
    qDebug("### finishRequest()");
    _msg = QString::fromLocal8Bit(_currentReq->body());
    _requestReceived = true;
    _currentReq = nullptr;

}
