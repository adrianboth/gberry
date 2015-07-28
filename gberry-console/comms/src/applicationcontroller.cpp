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
 
 #include "applicationcontroller.h"

#include <QFile>

// TODO: now linux only
#include <sys/types.h>
#include <signal.h>

#include <utils/fileutils.h>
#include <systemservices.h>
#include <utils/qtsignalproxy.h>
#include <resultmessageformatter.h>

#include "applicationexecutionsetup.h"
using namespace GBerryComms;

#define LOG_AREA "ApplicationController"
#include "log/log.h"

namespace {
    static int DEFAULT_PROCESS_KILL_WAIT_MS = 100;

    enum CurrentAction { NONE, LAUNCHING, RESUMING, STOPPING };
}

const char* ApplicationController::PROCESS_KILL_WAIT_MS_PROP = "processKillWaitMs";

// we avoid inheriting from QObject
class ApplicationControllerPrivate
{
public:
    ApplicationControllerPrivate(
            ApplicationController* q_,
            IApplicationExecutionSetup* executionSetup_) :
        q(q_),
        executionSetup(executionSetup_) {
        // these will be disconnect safely when *Private is destroyed
        // as 'process will be destroyed too.

        // old style connecting needed because func signatures cause problems
        QObject::connect(&process, SIGNAL(finished(int)),
                        &proxy, SLOT(proxyInt(int)));

        QObject::connect(&proxy, &QtSignalProxy::proxiedInt,
                         [this] (int code) { this->onProcessFinished(code);} );

        QObject::connect(&process, &QProcess::stateChanged,
                [this] (QProcess::ProcessState state) {
                    this->onProcessStateChanged(state); } );

        QObject::connect(&relaunchProxy, &QtSignalProxy::proxiedNoParameters,
                         [this] () { this->relaunchAfterDelay(); });
    }

    ApplicationController* q;
    QSharedPointer<IApplication> app;
    IApplicationExecutionSetup* executionSetup;
    QProcess process;
    QtSignalProxy proxy;
    QtSignalProxy relaunchProxy;

    int currentAction{NONE};
    bool running{false};
    bool simulated{false};
    int timerCalledCounterForWaitingProcessToStopRunning{0};

    void onProcessFinished(int exitCode) {
        DEBUG("Process finished with code: app_id = " << app->id() << ", exitCode =" << exitCode);
        if (running && currentAction != STOPPING) {
            DEBUG("Was expected to run");
            emit q->died();

        } else if (currentAction == STOPPING) {
            emit q->stopped();
        }

        currentAction = NONE;
        running = false;
        timerCalledCounterForWaitingProcessToStopRunning = 0;
    }

    void onProcessStateChanged(QProcess::ProcessState processState) {
        DEBUG("Process state changed: app_id =" << app->id());
        if (currentAction == LAUNCHING) {
            if (processState == QProcess::Running) {
                DEBUG("RUNNING OK");
                currentAction = NONE;
                running = true;
                emit q->launched();

            } else if (processState == QProcess::Starting) {
                DEBUG("STARTING");
                // no need to do anything

            } else if (processState == QProcess::NotRunning) {
                DEBUG("LAUNCH FAILED: " << process.errorString());
                currentAction = NONE;
                emit q->launchFailed();

            } else {
                ERROR("Unknown process state: " << processState);
            }

        }
    }

    // relaunch is done if original launch() call need to be delayed
    void relaunchAfterDelay() {
        q->launch();
    }
};


ApplicationController::ApplicationController(
        QSharedPointer<IApplication> app,
        IApplicationExecutionSetup* executionSetup,
        QObject *parent) :
    ApplicationController(executionSetup, parent)
{
    _d->app = app;
}

ApplicationController::ApplicationController(IApplicationExecutionSetup* executionSetup, QObject *parent) :
    IApplicationController(parent),
    _d(new ApplicationControllerPrivate(this, executionSetup))
{
    this->setProperty(PROCESS_KILL_WAIT_MS_PROP, DEFAULT_PROCESS_KILL_WAIT_MS);
}

ApplicationController::~ApplicationController()
{
}

void ApplicationController::launch()
{
    if (_d->simulated) {
        DEBUG("No real launch as running in simulated mode");
        emit launched();
        return;
    }

    if (_d->currentAction == STOPPING) {
        DEBUG("Process is still stopping, delaying launch to wait process first to stop");
        if (_d->timerCalledCounterForWaitingProcessToStopRunning == 3) {
            WARN("Previous process pid =" << _d->process.pid() << "fails to stop. Abort new launch.");
            // TODO: not sure how to clean up situation
            //  - retry stopping
            stop();
            emit launchFailed();
            return;
        }
        // stop initiated recently, wait process to die first
        int timeout = this->property(PROCESS_KILL_WAIT_MS_PROP).toInt();
        // only old style connection available
        SystemServices::instance()->singleshotTimer(timeout, &(_d->relaunchProxy), SLOT(proxyNoParameters()));
        _d->timerCalledCounterForWaitingProcessToStopRunning++; // wait three times
        return;
    }
    if (_d->app.isNull()) {
        WARN("Launch failed as request application is Null");
        emit launchFailed();
        return;
    }

    Result res;
    _d->executionSetup->prepare(_d->process, *_d->app.data(), res);
    if (res.hasErrors()) {
        ERROR(ResultMessageFormatter(res).createDeveloperMessage());
        emit launchFailed();
        return;
    }

    DEBUG("Launching application" << _d->app->id() << ", process: " << _d->process.program());
    _d->currentAction = LAUNCHING;
    _d->process.start();
}

void ApplicationController::pause()
{
    // TODO: impl
    if (_d->process.state() == QProcess::Running)
    {
        int result = kill(_d->process.pid(), SIGSTOP);
        DEBUG("Paused " << _d->app->id() << ", got result " << result);
        emit paused();
    }
}

void ApplicationController::resume()
{
    if (_d->process.state() == QProcess::Running)
    {
        int result = kill(_d->process.pid(), SIGCONT);
        DEBUG("Resumed " << _d->app->id() << ", got result " << result);
        emit resumed();
    }
}


void ApplicationController::stop()
{
    if (_d->process.state() == QProcess::Running) {
        DEBUG("Killing " << _d->app->id());
        _d->currentAction = STOPPING;
        _d->process.kill();
    }
}

void ApplicationController::setApplication(QSharedPointer<IApplication> app)
{
    _d->app = app;
}

QSharedPointer<IApplication> ApplicationController::application()
{
    return _d->app;
}

QString ApplicationController::fullApplicationId() const
{
    return _d->app->id();
}

void ApplicationController::enableSimulatedMode(bool enabled)
{
    _d->simulated = enabled;
}
