#ifndef MOCK_RESTINVOCATIONFACTORY_H
#define MOCK_RESTINVOCATIONFACTORY_H

#include "restinvocationfactory.h"

#include <gmock/gmock.h>


class MockRESTInvocationFactory : public RESTInvocationFactory
{
public:

    MOCK_METHOD0(newRESTInvocation, RESTInvocation*());
    MOCK_METHOD0(newDownloadStreamInvocation, DownloadStreamInvocation*());
};

#endif // MOCK_RESTINVOCATIONFACTORY_H

