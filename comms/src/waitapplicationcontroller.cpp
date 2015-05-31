#include "waitapplicationcontroller.h"

#include <QProcess>
#include <QDebug>

#define LOG_AREA "WaitApplicationController"
#include "log/log.h"

namespace {

}

WaitApplicationController::WaitApplicationController(QString& pathToApp, QObject *parent) :
    QObject(parent),
    _pathToApp(pathToApp),
    _state(NOT_RUNNING)

{
    DEBUG("Creating ...");
    _process.setProgram(_pathToApp);

    connect(&_process, SIGNAL(finished(int)),
                this, SLOT(onWaitApplicationFinished(int)));

    connect(&_process, &QProcess::stateChanged,
            this, &WaitApplicationController::onWaitApplicationRunning);
}

WaitApplicationController::~WaitApplicationController()
{

}

void WaitApplicationController::setMessage(QString &msg)
{
    // TODO: write to stdin of process
}

void WaitApplicationController::launch()
{
    DEBUG("Launching process");
    _process.start();
}


void WaitApplicationController::pause()
{
    DEBUG("Pause requested");
    // TODO: send signal
}

void WaitApplicationController::resume()
{
    DEBUG("Resume requested");
    // TODO: send signal
}

void WaitApplicationController::stop()
{
    _state = STOP;
    _process.kill();
}

void WaitApplicationController::onWaitApplicationRunning(QProcess::ProcessState processState)
{
    DEBUG("Process state")
    if (processState == QProcess::Running) {
        DEBUG("RUNNING");
        _state = RUN;
        emit launched();
    } else if (processState == QProcess::Starting) {
        DEBUG("STARTING");
    } else if (processState == QProcess::NotRunning) {
        DEBUG("NOT RUNNING: " << _process.errorString());
    } else {
        DEBUG("UNKNOWN");
    }

}

void WaitApplicationController::onWaitApplicationFinished(int exitCode)
{
    DEBUG("Process finished")
    if (_state == RUN) {
        DEBUG("Was expected to run");
    }
    // TODO: if stop was request then we shouldn't do anything
    // TODO: if died accidentally, we can't restart because of fb stack -> on next resume
}

// TODO: how to behave on desktop ... either start/kill, or hide/restore window
