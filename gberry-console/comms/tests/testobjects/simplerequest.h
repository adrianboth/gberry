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
 
#ifndef SIMPLEREQUEST_H
#define SIMPLEREQUEST_H

// Typically GTest *.cpp files don't need header files but
// Qt class definitions (inheriting from QObject) don't work
// well in *.cpp files, because Qt moc needs to analyse and
// generate classes. Of course alternative could be placing
// test classes to own *.h files but if there are many then
// having one common is more handy.

#include <QObject>
#include "invocation.h"
#include "restinvocation.h"
#include "request.h"

class SimpleRequest : public QObject, public GBerry::Request
{
    Q_OBJECT
public:
    int ProcessOkCallCount{0};
    int ProcessErrorCallCount{0};
    Invocation* LastInvocation{nullptr};
    Result LastError;

protected:
    virtual GBerry::Invocation* processPrepare(InvocationFactory* factory) override { return factory->newRESTInvocation(); }
    virtual void processOkResponse(Invocation* inv) override { LastInvocation = inv; ProcessOkCallCount++; }
    virtual void processErrorResponse(const Result& res, Invocation* inv) override { LastError = res; LastInvocation = inv; ProcessErrorCallCount++; }

};

#endif // SIMPLEREQUEST_H
