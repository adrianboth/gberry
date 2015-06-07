#ifndef LAUNCHCONTROLLER_H
#define LAUNCHCONTROLLER_H

#include "interfaces/ilaunchcontroller.h"
#include "server/application/iapplications.h"

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

    // returns true if managed to take application into use (it was found and valid info)
    virtual bool useApplication(const QString& appID);

private:
    IApplications* _apps;
    ApplicationController* _appController;
};

#endif // LAUNCHCONTROLLER_H
