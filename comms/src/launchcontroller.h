#ifndef LAUNCHCONTROLLER_H
#define LAUNCHCONTROLLER_H

#include "ilaunchcontroller.h"
#include "iapplications.h"
#include "applicationcontroller.h"

class LaunchController : public ILaunchController
{
    Q_OBJECT

public:
    LaunchController(IApplications* apps, QObject* parent = 0);
    ~LaunchController();

    virtual void launch();
    virtual void pause();
    virtual void resume();
    virtual void stop();

    virtual void useApplication(QString& appID);

private:
    IApplications* _apps;
    ApplicationController* _appController;
};

#endif // LAUNCHCONTROLLER_H
