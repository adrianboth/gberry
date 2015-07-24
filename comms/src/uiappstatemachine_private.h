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
 
 #ifndef UIAPPSTATEMACHINE_PRIVATE_H
#define UIAPPSTATEMACHINE_PRIVATE_H

// This file contains private implementation of UIAppStateMachine

#include <QObject>
#include <QState>
#include <QDebug>

#define LOG_AREA "UIAppStateMachine"
#include "log/log.h"

static const char* STATE_STARTUP = "startup";
static const char* STATE_WAITAPP_LAUNCHING_MAINUI = "waitAppVisibleLaunchingMainUI";
static const char* STATE_MAINUI = "mainuiVisible";
static const char* STATE_WAITAPP_LAUNCHING_APP = "waitAppVisibleLaunchingApp";
static const char* STATE_APP= "appVisible";

class UIAppStateMachinePrivate : public QObject
{
    Q_OBJECT

public:
    void emitAppLaunchRequested() { emit appLaunchRequested(); }
    void emitAppExitRequested()   { emit appExitRequested(); }
    void emitAppLaunchValidated() { emit appLaunchValidated(); }
    void emitWaitAppLaunchValidated() { emit waitappLaunchValidated(); }
    void emitMainUILaunchValidated()  { emit mainuiLaunchValidated(); }

signals:
    void appLaunchRequested();
    void appExitRequested();
    void waitappLaunchValidated();
    void mainuiLaunchValidated();
    void appLaunchValidated();

private:

};


class State : public QState
{
    Q_OBJECT

public:
    State(QString name = "") : QState(), _stateName(name) {
        connect(this, &QState::exited, this, &State::onExit);
    }
    ~State() {}

    QString name() const { return _stateName; }

signals:
    void entered();
    void enteredState(QString stateName);

protected:
    virtual void onEntry(QEvent* event) { TRACE("onEntry() to state" << _stateName); Q_UNUSED(event); emit entered(); emit enteredState(_stateName); }
    virtual void onExit() { TRACE("onExit() from state" << _stateName); }

private:
    QString _stateName;

};

#endif // UIAPPSTATEMACHINE_PRIVATE_H

