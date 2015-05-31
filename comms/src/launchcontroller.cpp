#include "launchcontroller.h"

LaunchController::LaunchController(IApplications* apps, QObject* parent) :
    ILaunchController(parent),
    _apps(apps)
{
    _appController = new ApplicationController(this);
}

LaunchController::~LaunchController()
{

}

void LaunchController::launch() { _appController->launch(); }
void LaunchController::pause() { _appController->pause(); }
void LaunchController::resume() { _appController->resume(); }
void LaunchController::stop() { _appController->stop(); }

void LaunchController::useApplication(QString& appID)
{
    // TODO: how to handle error, if no app found
    _appController->stop();
    _appController->setApplication(_apps->application(appID));
}
