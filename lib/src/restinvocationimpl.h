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
 
 #ifndef RESTINVOCATIONIMPL_H
#define RESTINVOCATIONIMPL_H

#include <QUrl>
#include <QScopedPointer>

#include "invocation.h"
#include "restinvocation.h"
#include "invocationfactoryimpl.h"
#include "httpinvocationdefinition.h"

using namespace GBerry;

namespace {
    static const uint DEFAULT_TIMEOUT_MS = 1000*30;
}

class RESTInvocationImplPrivate;

/**
 * @brief The RESTInvocationImpl class
 *
 * Properties:
 *   timeout
 *
 */
class RESTInvocationImpl : public RESTInvocation
{
public:
    RESTInvocationImpl(InvocationFactoryImpl* factory, QObject* parent = 0);
    virtual ~RESTInvocationImpl();

    // -- Invocation
    virtual void execute() override;
    virtual void abort() override;
    virtual Invocation::InvocationStatus statusCode() const;
    virtual Result result() const override;

    // -- RESTInvocation

    virtual void defineGetOperation(const QString& invocationPath) override;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) override;
    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const;
    virtual bool responseAvailable() const;
    virtual QByteArray responseByteData() const;
    virtual QString responseString() const;

private:
    const QScopedPointer<RESTInvocationImplPrivate> _d;
};

#endif // RESTINVOCATIONIMPL_H
