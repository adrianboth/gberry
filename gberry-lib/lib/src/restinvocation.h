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
 
 #ifndef RESTINVOCATION_H
#define RESTINVOCATION_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>

#include "invocation.h"
#include "httpinvocationdefinition.h"
using namespace GBerry;

/**
 * RESTInvocation is HTTP operation that has some restrictions that guide
 * it use to REST style operation, not just any HTTP request (you can use
 * QNetworkAccessManager directly to those).
 *
 * RESTInvocation is abstract class to separate interface and implementation.
 * The main benefit is to support unit testing.
 */
class RESTInvocation : public Invocation
{
    Q_OBJECT
public:
    explicit RESTInvocation(QObject *parent = 0);
    virtual ~RESTInvocation();

    // these are used to define operation
    virtual void defineGetOperation(const QString& invocationPath) = 0;
    virtual void definePostOperation(const QString& invocationPath, const QJsonDocument& jsondoc) = 0;
    virtual void defineParameter(const QString& paramName, const QString& paramValue) = 0;

    // TODO: could some of these moved to impl class?
    virtual HTTPInvocationDefinition::Status responseHttpStatusCode() const = 0;
    virtual bool responseAvailable() const = 0;
    virtual QByteArray responseByteData() const = 0;
    virtual QString responseString() const = 0;

    // TODO: json data response

};

#endif // RESTINVOCATION_H
