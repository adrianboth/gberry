#include "stubconsoleapplication.h"

StubConsoleApplication::StubConsoleApplication( InvocationFactory* invocationFactory_) :
    invocationFactory(invocationFactory_),
    userModelInstance(&tempDirApplicationStorage),
    loginModelInstance(&userModelInstance, &tempDirApplicationStorage, invocationFactory_)
{
}

StubConsoleApplication::~StubConsoleApplication()
{
}

IApplicationStorage& StubConsoleApplication::applicationStorage()
{
    return tempDirApplicationStorage;
}

UserModel& StubConsoleApplication::userModel()
{
    return userModelInstance;
}

LoginModel& StubConsoleApplication::loginModel()
{
    return loginModelInstance;
}
