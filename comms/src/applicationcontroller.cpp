#include "applicationcontroller.h"

#include <QFile>

// TODO: now linux only
#include <sys/types.h>
#include <signal.h>

#include "systemservices.h"

#define LOG_AREA "ApplicationController"
#include "log/log.h"

namespace {
    static int DEFAULT_PROCESS_KILL_WAIT_MS = 100;
}

const char* ApplicationController::PROCESS_KILL_WAIT_MS_PROP = "processKillWaitMs";

ApplicationController::ApplicationController(QSharedPointer<IApplicationMeta> meta, QObject *parent) :
    ApplicationController(parent)
{
    _appMeta = meta;
    this->setProperty(PROCESS_KILL_WAIT_MS_PROP, DEFAULT_PROCESS_KILL_WAIT_MS);
}

ApplicationController::ApplicationController(QObject *parent) :
    IApplicationController(parent),
    _currentAction(NONE),
    _running(false),
    _simulated(false),
    _timerForWaitingProcessToStopRunning(0)
{
    connect(&_process, SIGNAL(finished(int)),
                this, SLOT(onProcessFinished(int)));

    connect(&_process, &QProcess::stateChanged,
            this, &ApplicationController::onProcessStateChanged);
}

ApplicationController::~ApplicationController()
{

}

void ApplicationController::launch()
{
    if (_simulated) {
        DEBUG("No real launch as running in simulated mode");
        emit launched();
        return;
    }

    if (_currentAction == STOPPING) {
        DEBUG("Process is still stopping, delaying launch to wait process first to stop");
        if (_timerForWaitingProcessToStopRunning == 3) {
            WARN("Previous process pid =" << _process.pid() << "fails to stop. Abort new launch.");
            // TODO: not sure how to clean up situation
            //  - retry stopping
            stop();
            emit launchFailed();
            return;
        }
        // stop initiated recently, wait process to die first
        int timeout = this->property(PROCESS_KILL_WAIT_MS_PROP).toInt();
        SystemServices::instance()->singleshotTimer(timeout, this, SLOT(relaunchAfterDelay()));
        _timerForWaitingProcessToStopRunning += 1; // wait three times
        return;
    }
    if (_appMeta.isNull()) {
        WARN("Launch failed as request application is Null");
        emit launchFailed();
        return;
    }

    QString appExe = _appMeta->applicationExecutablePath();
    if (!QFile::exists(appExe)) {
        WARN("Launch failed as requested application executable not exists");
        emit launchFailed();
        return;
    }

    DEBUG("Launching process: " << appExe);
    _process.setProgram(appExe);
    _currentAction = LAUNCHING;
    _process.start();
}

void ApplicationController::relaunchAfterDelay()
{
    launch();
}

void ApplicationController::pause()
{
    // TODO: impl
    if (_process.state() == QProcess::Running)
    {
        int result = kill(_process.pid(), SIGSTOP);
        DEBUG("Paused " << _appMeta->id() << ", got result " << result);
    }
}

void ApplicationController::resume()
{
    if (_process.state() == QProcess::Running)
    {
        int result = kill(_process.pid(), SIGCONT);
        DEBUG("Resumed " << _appMeta->id() << ", got result " << result);
    }
}


void ApplicationController::stop()
{
    if (_process.state() == QProcess::Running) {
        DEBUG("Killing " << _appMeta->id());
        _currentAction = STOPPING;
        _process.kill();
    }
}

void ApplicationController::setApplication(QSharedPointer<ApplicationMeta> meta)
{
    _appMeta = meta;
}

void ApplicationController::onProcessStateChanged(QProcess::ProcessState processState)
{
    DEBUG("Process state changed: app_id =" << _appMeta->id());
    if (_currentAction == LAUNCHING) {
        if (processState == QProcess::Running) {
            DEBUG("RUNNING OK");
            _currentAction = NONE;
            _running = true;
            emit launched();

        } else if (processState == QProcess::Starting) {
            DEBUG("STARTING");
            // no need to do anything

        } else if (processState == QProcess::NotRunning) {
            DEBUG("LAUNCH FAILED: " << _process.errorString());
            _currentAction = NONE;
            emit launchFailed();

        } else {
            ERROR("Unknown process state: " << processState);
        }

    }

}

void ApplicationController::onProcessFinished(int exitCode)
{
    DEBUG("Process finished with code: app_id = " << _appMeta->id() << ", exitCode =" << exitCode);
    if (_running && _currentAction != STOPPING) {
        DEBUG("Was expected to run");
        emit died();

    } else if (_currentAction == STOPPING) {
        emit stopped();
    }

    _currentAction = NONE;
    _running = false;
    _timerForWaitingProcessToStopRunning = 0;
}

void ApplicationController::enableSimulatedMode(bool enabled)
{
    _simulated = enabled;
}
