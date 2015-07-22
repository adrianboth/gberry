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
