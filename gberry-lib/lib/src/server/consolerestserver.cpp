/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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

#define LOG_AREA "ConsoleRESTServer"
#include "log/log.h"


ConsoleRESTServer::ConsoleRESTServer(PlayerSessionManager &sessionManager) :
    _sessionManager(sessionManager), _tokenCounter(100)
{
    _server = new QHttpServer(this);
    connect(_server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    // TODO: configurable port
    int port = 8050;
    _server->listen(QHostAddress::Any, port);
    DEBUG("Listening on 0.0.0.0:" << port);
}

ConsoleRESTServer::~ConsoleRESTServer()
{
    _server->close();
}

void ConsoleRESTServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    TRACE("handleRequest()");

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
        DEBUG("ERROR 403");
    }
}


void ConsoleRESTServer::handlePostRequest(QHttpRequest *req, QHttpResponse *resp)
{
    // at this point http request full body is available

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
                QString playerName = json["name"].toString();

                // identical names are not allow
                // TODO: should each player has some kind of nick that can be adjusted, or even several ones
                if (_sessionManager.isPlayerNameReserved(playerName))
                {
                    // TODO: return error, json
                    resp->writeHead(403);
                    // TODO: how to localize these error messages?
                    QString msg("Player nick name %1 already used. Please provide other one.");
                    msg = msg.arg(playerName);

                    resp->end(QByteArray(msg.toLatin1()));
                    // TODO: this if()-else() could be refactored to some nicer...
                    return;
                }

                // TODO: generate more compled session token
                QString sessionToken("abc");
                sessionToken.append(QString::number(_tokenCounter++));

                PlayerSession session;
                if (id == "guest") {
                    session = GuestPlayerSession(playerName, sessionToken);
                } else {
                    // id = userToken
                    session = SignedInPlayerSession(playerName, sessionToken, id);
                }

                _sessionManager.insertSession(session);

                QJsonObject respJson;
                respJson["response_type"] = "ok";
                respJson["token"] = session.sessionToken();

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
        DEBUG("ERROR 403");
    }

}
