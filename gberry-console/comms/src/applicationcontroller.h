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
 
 #ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QScopedPointer>

#include "interfaces/iapplicationcontroller.h"
#include "server/application/iapplication.h"
#include "server/applicationregistry.h"

using namespace GBerry::Console::Server;

class ApplicationControllerPrivate;

class ApplicationController : public IApplicationController
{
    Q_OBJECT

public:
    explicit ApplicationController(
            QSharedPointer<IApplication> app,
            ApplicationRegistry* registry,
            QObject *parent = 0);
    ApplicationController(QObject *parent = 0);
    ~ApplicationController();

    // IApplicationController
    virtual void launch() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void stop() override;
    // TODO: this refers to full-id (name + version), could there be better name
    virtual QString fullApplicationId() const override;

    // --
    virtual void setApplication(QSharedPointer<IApplication> app);
    virtual QSharedPointer<IApplication> application();
    virtual void enableSimulatedMode(bool enabled);

    virtual void enableOutputLogging(bool enabled);

    // -- dynamic properties
    static const char* PROCESS_KILL_WAIT_MS_PROP;

signals:

public slots:

private:
    const QScopedPointer<ApplicationControllerPrivate> _d;

};


#endif // APPLICATIONCONTROLLER_H
