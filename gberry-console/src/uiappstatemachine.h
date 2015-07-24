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
 
 #ifndef UIAPPSTATEMACHINE_H
#define UIAPPSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QDebug>

#include "interfaces/iapplicationcontroller.h"
#include "interfaces/ilaunchcontroller.h"

class UIAppStateMachinePrivate;

/**
 * This takes care of starting waiting screen, mainui, and requested application.
 *
 * It setups necessary watchdogs to monitor that applications are running
 */
class UIAppStateMachine : public QObject
{
    Q_OBJECT

public:
    UIAppStateMachine(IApplicationController* waitApp,
                      IApplicationController* mainui,
                      ILaunchController* currentApp);

    ~UIAppStateMachine();


    void start();

    // TODO: this should be actually a class presenting applications
    void lauchApplication(const QString& applicationId);
    void exitApplication(const QString& applicationId);

    // TODO: how to signal error situations for user

    // utility for testing/debugging
    QString debugCurrentStateName() const;

public slots:
    void applicationConnectionValidated(const QString& applicationId);

private:
    QStateMachine _stateMachine;
    IApplicationController* _waitapp;
    IApplicationController* _mainui;
    ILaunchController* _currentApp;

    UIAppStateMachinePrivate* _impl;
    QString _currentStateName;

};


#endif // UIAPPSTATEMACHINE_H
