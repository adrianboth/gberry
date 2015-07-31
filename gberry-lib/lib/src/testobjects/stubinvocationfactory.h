#ifndef STUBINVOCATIONFACTORY_H
#define STUBINVOCATIONFACTORY_H

#include "invocationfactory.h"
#include "stubrestinvocation.h"
#include "stubdownloadstreaminvocation.h"


class StubInvocationFactory : public InvocationFactory
{
public:
    StubInvocationFactory();
    ~StubInvocationFactory();

    virtual RESTInvocation* newRESTInvocation();
    virtual DownloadStreamInvocation* newDownloadStreamInvocation();
    virtual QString defaultHostName() const;

    QList<StubRestInvocation*> restInvs;
    QList<StubDownloadStreamInvocation*> downloadInvs;
};

#endif // STUBINVOCATIONFACTORY_H
