#ifndef TEST_REQUEST_H
#define TEST_REQUEST_H

// Typically GTest *.cpp files don't need header files but
// Qt class definitions (inheriting from QObject) don't work
// well in *.cpp files, because Qt moc needs to analyse and
// generate classes. Of course alternative could be placing
// test classes to own *.h files but if there are many then
// having one common is more handy.

#include <QObject>
#include "request.h"

class SimpleRequest : public QObject, public GBerry::Request
{
    Q_OBJECT
public:
    int processOkCallCount{0};
    int processErrorCallCount{0};
    RESTInvocation* lastInvocation{nullptr};

protected:
    virtual void processPrepare(RESTInvocation* inv) override { Q_UNUSED(inv); }
    virtual void processOkResponse(RESTInvocation* inv) override { Q_UNUSED(inv); processOkCallCount++; }
    virtual void processErrorResponse(RESTInvocation* inv) override { Q_UNUSED(inv); processErrorCallCount++; }

};

#endif // TEST_REQUEST_H
