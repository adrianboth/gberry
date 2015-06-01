#include "applicationcontroller.h"

// TODO: now linux only
#include <sys/types.h>
#include <signal.h>

#define LOG_AREA "ApplicationController"
#include "log/log.h"

ApplicationController::ApplicationController(QPointer<ApplicationMeta> meta, QObject *parent) :
    ApplicationController(parent)
{
    _appMeta = meta;
}

ApplicationController::ApplicationController(QObject *parent) :
    IApplicationController(parent),
    _currentAction(NONE),
    _running(false),
    _simulated(false)
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

    if (_appMeta) {
        DEBUG("Launching process: " << _appMeta->applicationPath());
        _process.setProgram(_appMeta->applicationPath());
        _currentAction = LAUNCHING;
        _process.start();
    } else {
        WARN("No ApplicationMeta defined, nothing to start")
    }

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

void ApplicationController::setApplication(QPointer<ApplicationMeta> meta)
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
        _running = false;
        emit died();

    } else if (_currentAction == STOPPING) {
        _currentAction = NONE;
        emit stopped();
    }
}

void ApplicationController::enableSimulatedMode(bool enabled)
{
    _simulated = enabled;
}
