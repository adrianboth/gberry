#ifndef REQUEST_H
#define REQUEST_H

#include "invocation.h"
#include "invocationfactory.h"

namespace GBerry {

class Request
{
public:
    explicit Request();
    virtual ~Request();

    enum Error { ERROR_NO_CONNECTION, ERROR_INVOCATION_FAILED, ERROR_INVALID_JSON_RESPONSE };

    Invocation* prepareInvocation(InvocationFactory* invocationFactory);

    void finishedOk(Invocation* invocation);

    // NOTE: 'invocation' can be null if Error occurs before 'invocation' has been
    //        created.
    void finishedError(Error err, Invocation* invocation);

    // Requests need to be alive as long as RESTInvocation is live. Calling
    // cancel() means that this Request is not longer needed and can be
    // self-destroyed when RESTInvocation finishes.
    virtual void cancel();

protected:
    virtual Invocation* processPrepare(InvocationFactory* factory) = 0;
    virtual void processOkResponse(Invocation* invocation) = 0;
    virtual void processErrorResponse(Error error, Invocation* invocation) = 0;

    Invocation* _invocation{nullptr};
    bool _active{true};

};

} // eon

#endif // REQUEST_H
