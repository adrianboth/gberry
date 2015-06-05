#ifndef ILAUNCHCONTROLLER_H
#define ILAUNCHCONTROLLER_H

#include "applicationcontroller.h"

class ILaunchController : public IApplicationController
{
    Q_OBJECT
public:
    ILaunchController(QObject* parent = 0) : IApplicationController(parent) {}
    ~ILaunchController() {}

    virtual bool useApplication(const QString& appID) = 0;
};

#endif // ILAUNCHCONTROLLER_H

