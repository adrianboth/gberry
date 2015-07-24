/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
    connect(_appController, &ApplicationController::stopped, [&] () { emit this->stopped(); });
    connect(_appController, &ApplicationController::died, [&] () { emit this->died(); });
}

LaunchController::~LaunchController()
{
}

void LaunchController::launch() { _appController->launch(); }
void LaunchController::pause() { _appController->pause(); }
void LaunchController::resume() { _appController->resume(); }
void LaunchController::stop() { _appController->stop(); }

QString LaunchController::fullApplicationId() const
{
   return  _appController->fullApplicationId();
}

bool LaunchController::useApplication(const QString& appID)
{
    _appController->stop();
    QSharedPointer<IApplication> app(_apps->application(appID));
    _appController->setApplication(app);
    // enabling logging is not stricly required when we update app, but to
    // this is to highlight that log will go to app specific dir
    _appController->enableOutputLogging(true);
    return !app.isNull();
}
