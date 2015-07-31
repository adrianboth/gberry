#include "stubinvocationfactory.h"

StubInvocationFactory::StubInvocationFactory()
{

}

StubInvocationFactory::~StubInvocationFactory()
{

}

RESTInvocation *StubInvocationFactory::newRESTInvocation()
{
    StubRestInvocation* inv = new StubRestInvocation;
    restInvs.append(inv);
    return inv;
}

DownloadStreamInvocation *StubInvocationFactory::newDownloadStreamInvocation()
{
    StubDownloadStreamInvocation* inv = new StubDownloadStreamInvocation;
    downloadInvs.append(inv);
    return inv;
}

QString StubInvocationFactory::defaultHostName() const
{
    // TODO: impl
    return "not implemented";
}
