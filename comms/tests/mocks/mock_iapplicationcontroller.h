#ifndef MOCK_IAPPLICATIONCONTROLLER_H
#define MOCK_IAPPLICATIONCONTROLLER_H


#include "interfaces/iapplicationcontroller.h"
#include "interfaces/ilaunchcontroller.h"

#include <gmock/gmock.h>

//#define MOCK_SIGNAL(func) void emit_##func() {  ##func() ); }

class MockIApplicationController : public IApplicationController
{
    Q_OBJECT
public:
    MOCK_METHOD0(launch, void());
    MOCK_METHOD0(pause, void());
    MOCK_METHOD0(resume, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(fullApplicationId, QString());

    void emitLaunched() { emit launched(); }
};

class MockILaunchController : public ILaunchController
{
    Q_OBJECT
public:
    MOCK_METHOD0(launch, void());
    MOCK_METHOD0(pause, void());
    MOCK_METHOD0(resume, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(fullApplicationId, QString());
    MOCK_METHOD1(useApplication, bool(const QString& s));

    void emitLaunched() { emit launched(); }
    void emitLaunchFailed() { emit launchFailed(); }
    void emitDied() { emit died(); }
};

#endif // MOCK_IAPPLICATIONCONTROLLER_H

