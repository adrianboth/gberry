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
 
 #ifndef DOWNLOADSTREAMINVOCATION_H
#define DOWNLOADSTREAMINVOCATION_H

#include <QUrl>
#include <QScopedPointer>

#include "invocation.h"
#include "invocationfactoryimpl.h"
#include "httpinvocationdefinition.h"

namespace GBerry {

/**
 * This is pretty much as typical RESTInvocation but instead storing response
 * data to memory it is streamed a file.
 */
class DownloadStreamInvocation : public Invocation
{
    Q_OBJECT

public:
    explicit DownloadStreamInvocation(QObject* parent = 0);
    virtual ~DownloadStreamInvocation();

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) = 0;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) = 0;

    // TODO: later support for stream (possible to continue writing)
    virtual void setOutputFilePath(QString filePath) = 0;

    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const = 0;

    virtual int progressPercentage() const = 0;

signals:
    void downloadStarted(DownloadStreamInvocation* inv);
    //void downloadFinished(DownloadStreamInvocation* inv);
    //void downloadError(DownloadStreamInvocation* inv);
    void downloadProgress(DownloadStreamInvocation* inv);

};

} // eon

#endif // DOWNLOADSTREAMINVOCATION_H
