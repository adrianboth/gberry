#ifndef IOTHERSERVICE
#define IOTHERSERVICE

#include <QtPlugin>
#include <QString>

class IOtherService
{
public:
    //virtual ~IOtherService() {}

    virtual QString publicFunc() = 0;

    virtual void publicFunc2() = 0;

};

Q_DECLARE_INTERFACE(IOtherService, "gberry.otherservice/1.0")

#endif // IOTHERSERVICE

