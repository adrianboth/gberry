#ifndef ICONSOLEAPPLICATION_H
#define ICONSOLEAPPLICATION_H

#include "client/iapplicationstorage.h"
#include "client/qml/usermodel.h"
#include "client/qml/loginmodel.h"

namespace GBerryClient {

class IConsoleApplication
{
public:
    IConsoleApplication() {}
    virtual ~IConsoleApplication() {}

    virtual IApplicationStorage& applicationStorage() = 0;
    virtual UserModel& userModel() = 0;
    virtual LoginModel& loginModel() = 0;

};

} // eon

#endif // ICONSOLEAPPLICATION_H

