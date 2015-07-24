#ifndef MOCK_IOTHERSERVICE_H
#define MOCK_IOTHERSERVICE_H

#include <gmock/gmock.h>

#include <otherservice.h>

// note: If you fail to mock all functions e.g.
//        - wrong return type
//        - wrong parameters (either type or MOCK_METHODx)
//       then you easily get cryptic compilation errors.
//
class MockIOtherService : public IOtherService
{
 public:
     MOCK_METHOD0(publicFunc, QString());
     MOCK_METHOD0(publicFunc2, void());
};

#endif // MOCK_IOTHERSERVICE_H
