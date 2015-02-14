#include "gberryconsoleserver.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

GBerryConsoleServer::GBerryConsoleServer()
{
    _server = new QHttpServer(this);
    connect(_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    // TODO: configurable port
    _server->listen(QHostAddress::Any, 8050);
    qDebug() << "SERVER: Listening on  0.0.0.0:8050";
}

GBerryConsoleServer::~GBerryConsoleServer()
{
    _server->close();
}

void GBerryConsoleServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    qDebug() << "INCOMING REQUEST";
    QRegExp exp("^/console/v1/([a-z]+)?$");
    if( exp.indexIn(req->path()) != -1 )
    {
        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);

        QString name = exp.capturedTexts()[1];
        QString body = tr("<html><head><title>Greeting App</title></head><body><h1>Hello %1!</h1></body></html>");
        resp->end(body.arg(name).toUtf8());
        qDebug() << "OK";
    }
    else
    {
        resp->writeHead(403);
        resp->end(QByteArray("You aren't allowed here!"));
        qDebug() << "ERROR 403";
    }
}
