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

    virtual void launch() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void stop() override;
    virtual QString fullApplicationId() const override;

    // returns true if managed to take application into use (it was found and valid info)
    virtual bool useApplication(const QString& appID) override;

private:
    IApplications* _apps;
    ApplicationController* _appController;
};

#endif // LAUNCHCONTROLLER_H
