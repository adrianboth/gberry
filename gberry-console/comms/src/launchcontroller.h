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
 
 #ifndef LAUNCHCONTROLLER_H
#define LAUNCHCONTROLLER_H

#include "interfaces/ilaunchcontroller.h"
#include "server/application/iapplications.h"

class LaunchController : public ILaunchController
{
    Q_OBJECT

public:
    LaunchController(IApplications* apps,
                     IApplicationExecutionSetup* executionSetup,
                     QObject* parent = 0);
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
