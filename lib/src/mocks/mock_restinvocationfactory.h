#ifndef MOCK_RESTINVOCATIONFACTORY_H
#define MOCK_RESTINVOCATIONFACTORY_H

#include "restinvocationfactory.h"

#include <gmock/gmock.h>


class MockRESTInvocationFactory : public RESTInvocationFactory
{
public:

    MOCK_METHOD0(newInvocation, RESTInvocation*());
};

#endif // MOCK_RESTINVOCATIONFACTORY_H

