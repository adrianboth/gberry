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

    virtual void prepare(RESTInvocation* invocation) = 0;
    virtual void finishedOk(RESTInvocation* invocation) = 0;
    virtual void finishedFailure(RESTInvocation* invocation) = 0;
};

} // eon

#endif // REQUEST_H
