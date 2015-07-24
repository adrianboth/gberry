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
 
 #ifndef INVOCATION_H
#define INVOCATION_H

#include <QObject>

#include "result.h"
using namespace GBerryLib;

namespace GBerry {

ERRORCLASS(InvocationError)

class InvocationErrors {
public:
   static const InvocationError UNSPECIFIED;
   static const InvocationError INVOCATION_INVALID;
   static const InvocationError INVOCATION_FAILED;
   static const InvocationError CONNECTION_FAILED;
   static const InvocationError CONNECTION_TIMEOUT;
};

class Invocation : public QObject
{
    Q_OBJECT

public:
    explicit Invocation(QObject *parent = 0);
    virtual ~Invocation();

    // TODO: maybe just "Status"
    enum InvocationStatus {
        NOT_STARTED,
        INVALID_INVOCATION, // when parameters of invocation as invalid
        ONGOING,
        CONNECTION_FAILED,
        TIMEOUT_OCCURRED,
        ERROR,
        RESPONSE_RECEIVED,
        FINISHED
    };

    // query will be executed
    virtual void execute() = 0;

    // aborts ongoing request
    virtual void abort() = 0;

    // TODO: could some of these moved to impl class?
    virtual InvocationStatus statusCode() const = 0;

    virtual Result result() const = 0;

signals:
    void finishedOK(Invocation* invocation);
    void finishedError(Invocation* invocation);

public slots:

};

} // eon

#endif // INVOCATION_H
