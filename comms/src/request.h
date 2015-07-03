#ifndef REQUEST_H
#define REQUEST_H

class RESTInvocation;

namespace GBerry {

class Request
{
public:
    explicit Request();
    virtual ~Request();

    // TODO: how to get download stuff -> streaming

    void prepare(RESTInvocation* invocation);

    void finishedOk(RESTInvocation* invocation);
    void finishedError(RESTInvocation* invocation);

    // Requests need to be alive as long as RESTInvocation is live. Calling
    // cancel() means that this Request is not longer needed and can be
    // self-destroyed when RESTInvocation finishes.
    virtual void cancel();

protected:
    virtual void processPrepare(RESTInvocation* invocation) = 0;
    virtual void processOkResponse(RESTInvocation* invocation) = 0;
    virtual void processErrorResponse(RESTInvocation* invocation) = 0;

    RESTInvocation* _invocation{nullptr};
    bool _active{true};

};

} // eon

#endif // REQUEST_H
