#include "consolerestserver.h"


#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "server/playersession.h"


ConsoleRESTServer::ConsoleRESTServer(PlayerSessionManager &sessionManager) :
    _sessionManager(sessionManager), _tokenCounter(100)
{
    _server = new QHttpServer(this);
    connect(_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    // TODO: configurable port
    int port = 8050;
    _server->listen(QHostAddress::Any, port);
    qDebug() << "[ConsoleRESTServer]: Listening on  0.0.0.0:" << port;
}

ConsoleRESTServer::~ConsoleRESTServer()
{
    _server->close();
}

void ConsoleRESTServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    qDebug() << "[ConsoleRESTServer] INCOMING REQUEST";

    if (req->method() == QHttpRequest::HTTP_POST)
    {
        // wait all data has arrived
        connect(req, &QHttpRequest::end,
                [this,req,resp] () { handlePostRequest(req, resp); } );
        req->storeBody();
        return;
    }

    // -- GET and others
    QRegExp exp("^/console/v1/([a-z]+)?$");
    if( exp.indexIn(req->path()) != -1 )
    {
        QString name = exp.capturedTexts()[1];
        if (name == "ping")
        {
            resp->setHeader("Content-Type", "text/html");
            resp->writeHead(200);
            resp->end(QString("pong").toUtf8());
        }
        else
        {
            resp->writeHead(403);
            resp->end(QByteArray("Not supported url"));
        }
    }
    else
    {
        resp->writeHead(403);
        resp->end(QByteArray("You aren't allowed here!"));
        qDebug() << "ERROR 403";
    }
}


void ConsoleRESTServer::handlePostRequest(QHttpRequest *req, QHttpResponse *resp)
{
    // at this point http request full body is available
    //QString body = QString::fromLocal8Bit(req->body());

    QRegExp exp("^/console/v1/([a-z]+)?$");
    if( exp.indexIn(req->path()) != -1 )
    {

        QString name = exp.capturedTexts()[1];
        if (name == "ping")
        {
            if (req->header("Content-Type") != "application/json")
            {
                resp->writeHead(403);
                QString msg("Unsupported content type: ");
                msg.append(req->header("Content-Type"));
                resp->end(QByteArray(msg.toLatin1()));
                return;
            }

            QJsonDocument jsondoc(QJsonDocument::fromJson(req->body()));

            // echo back
            resp->setHeader("Content-Type", "application/json");
            resp->writeHead(200);
            resp->end(jsondoc.toJson());
        }
        else if (name == "session")
        {
            QJsonDocument jsondoc(QJsonDocument::fromJson(req->body()));
            QJsonObject json = jsondoc.object();
            if (json.contains("action") && json["action"] == "open_console_session")
            {
                // TODO: now just guests
                QString id = json["id"].toString();
                QString name = json["name"].toString();

                // identical names are not allow
                // TODO: should each player has some kind of nick that can be adjusted, or even several ones
                if (_sessionManager.isPlayerNameReserved(name))
                {
                    // TODO: return error, json
                    resp->writeHead(403);
                    // TODO: how to localize these error messages?
                    QString msg("Player nick name %1 already used. Please provide other one.");
                    msg = msg.arg(name);

                    resp->end(QByteArray(msg.toLatin1()));
                    // TODO: this if()-else() could be refactored to some nicer...
                    return;
                }
                QString token("abc");
                token.append(QString::number(_tokenCounter++));
                GuestPlayerSession session(name, token); // TODO: just fixed token
                _sessionManager.insertSession(session);

                QJsonObject respJson;
                respJson["response_type"] = "ok";
                respJson["token"] = session.token();

                resp->setHeader("Content-Type", "application/json");
                resp->writeHead(200);
                resp->end(QJsonDocument(respJson).toJson());
            }
            else
            {
                // validation error
                resp->writeHead(403);
                QString msg("Invalid json message");
                resp->end(QByteArray(msg.toLatin1()));
            }
        }
        else
        {
            resp->writeHead(403);
            QString msg("Not supported url: ");
            msg.append(name);
            resp->end(QByteArray(msg.toLatin1()));
        }

    }
    else
    {
        resp->writeHead(403);
        resp->end(QByteArray("You aren't allowed here!"));
        qDebug() << "ERROR 403";
    }

}
