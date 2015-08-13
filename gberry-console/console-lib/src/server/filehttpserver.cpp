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

#include "filehttpserver.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include <utils/fileutils.h>

#define LOG_AREA "FileHttpServer"
#include "log/log.h"

namespace GBerryComms {

FileHttpServer::FileHttpServer(int port, const QString& path) :
    _rootDir(path)
{
    DEBUG("Document root dir = " << _rootDir);
    if (!QFileInfo(_rootDir).exists()) {
        ERROR("Root dir doesn't exist");
    } else if (!QFileInfo(GBerryLib::joinpath(_rootDir, "index.html")).exists()) {
        ERROR("No index.html file under root dir");
    } else {

        QHttpServer *server = new QHttpServer(this);
        connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
                this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

        server->listen(QHostAddress::Any, port);
        DEBUG("Listering 0.0.0.0 :" << port);
    }
}

void FileHttpServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    QRegExp exp("^/(.+)$");
    if( exp.indexIn(req->path()) != -1 )
    {
        // looking for specific file
        QFile file(GBerryLib::joinpath(_rootDir, exp.capturedTexts()[1]));
        if (!file.open(QIODevice::ReadOnly)) {
            ERROR("Failed to read file: " + file.fileName());
            resp->writeHead(404);
            resp->setHeader("Content-Type", "text/plain");
            QString data = "Failed to find " + exp.capturedTexts()[1];
            resp->end(data.toLatin1());
        } else {
            QByteArray data = file.readAll();
            resp->writeHead(200);
            if (QFileInfo(file.fileName()).suffix() == "html") {
                resp->setHeader("Content-Type", "text/html");
            } else if (QFileInfo(file.fileName()).suffix() == "txt") {
                resp->setHeader("Content-Type", "text/plain");
            } else if (QFileInfo(file.fileName()).suffix() == "apk") {
                resp->setHeader("Content-Type", "application/octet-stream");
            } else if (QFileInfo(file.fileName()).suffix() == "zip") {
                resp->setHeader("Content-Type", "application/zip");

            }

            resp->end(data);
        }

    }
    else
    {
        resp->setHeader("Content-Type", "text/html");

        QFile indexFile(GBerryLib::joinpath(_rootDir, "index.html"));
        if (!indexFile.open(QIODevice::ReadOnly)) {
            ERROR("Failed to read file: " + indexFile.fileName());
            resp->writeHead(404);
            resp->end(QByteArray("Failed to open index.html"));
        } else {
            QByteArray data = indexFile.readAll();
            resp->writeHead(200);
            resp->end(data);
        }
    }
}

} // eon
