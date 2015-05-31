#include "applicationcontroller.h"

#define LOG_AREA "ApplicationController"
#include "log/log.h"

ApplicationController::ApplicationController(QPointer<ApplicationMeta> meta, QObject *parent) :
    ApplicationController(parent)
{
    _appMeta = meta;
    connect(&_process, SIGNAL(finished(int)),
                this, SLOT(onProcessFinished(int)));

    connect(&_process, &QProcess::stateChanged,
            this, &ApplicationController::onProcessStateChanged);

}

ApplicationController::ApplicationController(QObject *parent) :
    IApplicationController(parent),
    _currentAction(NONE),
    _running(false)
{

}

ApplicationController::~ApplicationController()
{

}

void ApplicationController::launch()
{
    DEBUG("Launching process: " << _appMeta->applicationPath());
    _process.setProgram(_appMeta->applicationPath());
    _currentAction = LAUNCHING;
    _process.start();
}

void ApplicationController::pause()
{
    // TODO: impl
}

void ApplicationController::resume()
{
    // TODO: impl
}


void ApplicationController::stop()
{
    _currentAction = NONE;
    _process.kill();
}

void ApplicationController::setApplication(QPointer<ApplicationMeta> meta)
{
    _appMeta = meta;
}

void ApplicationController::onProcessStateChanged(QProcess::ProcessState processState)
{
    DEBUG("Process state changed")
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
    DEBUG("Process finished with code: " << exitCode)
    if (_running ) {
        DEBUG("Was expected to run");
        _running = false;
        emit died();
    }
}
