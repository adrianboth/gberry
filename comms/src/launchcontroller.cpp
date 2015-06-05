#include "launchcontroller.h"

LaunchController::LaunchController(IApplications* apps, QObject* parent) :
    ILaunchController(parent),
    _apps(apps)
{
    _appController = new ApplicationController(this);
    connect(_appController, &ApplicationController::launched, [&] () { emit this->launched(); });
    connect(_appController, &ApplicationController::launchFailed, [&] () { emit this->launchFailed(); });
    connect(_appController, &ApplicationController::resumed, [&] () { emit this->resumed(); });
    connect(_appController, &ApplicationController::resumeFailed, [&] () { emit this->resumeFailed(); });
    connect(_appController, &ApplicationController::died, [&] () { emit this->died(); });
}

LaunchController::~LaunchController()
{

}

void LaunchController::launch() { _appController->launch(); }
void LaunchController::pause() { _appController->pause(); }
void LaunchController::resume() { _appController->resume(); }
void LaunchController::stop() { _appController->stop(); }

bool LaunchController::useApplication(const QString& appID)
{
    // TODO: how to handle error, if no app found
    _appController->stop();
    //QSharedPointer<ApplicationMeta> _apps->application(appID);

    _appController->setApplication(_apps->application(appID));
// TODO:
    return true;
}
