#ifndef MOCK_RESTINVOCATIONFACTORY_H
#define MOCK_RESTINVOCATIONFACTORY_H

#include "invocationfactory.h"

#include <gmock/gmock.h>


class MockRESTInvocationFactory : public InvocationFactory
{
public:

    MOCK_METHOD0(newRESTInvocation, RESTInvocation*());
    MOCK_METHOD0(newDownloadStreamInvocation, DownloadStreamInvocation*());
};

#endif // MOCK_RESTINVOCATIONFACTORY_H

