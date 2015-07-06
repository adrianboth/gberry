#ifndef TEST_REQUEST_H
#define TEST_REQUEST_H

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
    Error LastError;

protected:
    virtual GBerry::Invocation* processPrepare(InvocationFactory* factory) override { return factory->newRESTInvocation(); }
    virtual void processOkResponse(Invocation* inv) override { LastInvocation = inv; ProcessOkCallCount++; }
    virtual void processErrorResponse(Error err, Invocation* inv) override { LastError = err; LastInvocation = inv; ProcessErrorCallCount++; }

};

#endif // TEST_REQUEST_H
