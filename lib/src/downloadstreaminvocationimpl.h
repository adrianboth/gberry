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
 
 #ifndef DOWNLOADSTREAMINVOCATIONIMPL_H
#define DOWNLOADSTREAMINVOCATIONIMPL_H

#include <QUrl>
#include <QScopedPointer>

#include "downloadstreaminvocation.h"
// TODO: change name of factory
#include "invocationfactoryimpl.h"

namespace GBerry {

class DownloadStreamInvocationImplPrivate;

/**
 * This is pretty much as typical RESTInvocation but instead storing response
 * data to memory it is streamed a file.
 */
class DownloadStreamInvocationImpl : public DownloadStreamInvocation
{
    Q_OBJECT

public:
    DownloadStreamInvocationImpl(InvocationFactoryImpl* factory, QObject* parent = 0);
    virtual ~DownloadStreamInvocationImpl();

    // -- Invocation
    virtual void execute() override;
    virtual void abort() override;
    virtual Result result() const override;
    virtual Invocation::InvocationStatus statusCode() const override;

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) override;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override;

    // TODO: later support for stream (possible to continue writing)
    virtual void setOutputFilePath(QString filePath);

    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const override;

    virtual int progressPercentage() const override;

private:
    const QScopedPointer<DownloadStreamInvocationImplPrivate> _d;

};

} // eon

#endif // DOWNLOADSTREAMINVOCATIONIMPL_H
