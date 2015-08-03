#ifndef STUBCONSOLEAPPLICATION_H
#define STUBCONSOLEAPPLICATION_H

#include "client/iconsoleapplication.h"
#include "client/qml/usermodel.h"
#include "client/qml/loginmodel.h"

#include "testobjects/stubapplicationstorage.h"

using namespace GBerryClient;

class StubConsoleApplication : public IConsoleApplication
{
public:
    StubConsoleApplication(InvocationFactory* invocationFactory);
    ~StubConsoleApplication();

    virtual IApplicationStorage& applicationStorage() override;
    virtual UserModel& userModel() override;
    virtual LoginModel& loginModel() override;


    TempDirApplicationStorage tempDirApplicationStorage;
    InvocationFactory* invocationFactory;
    UserModel userModelInstance;
    LoginModel loginModelInstance;
};

#endif // STUBCONSOLEAPPLICATION_H
