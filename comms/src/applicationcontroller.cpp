#include "applicationcontroller.h"

#include <QFile>

// TODO: now linux only
#include <sys/types.h>
#include <signal.h>

#include "systemservices.h"
#include <utils/qtsignalproxy.h>

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
    ApplicationControllerPrivate(ApplicationController* q_) :
        q(q_) {
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
    ApplicationRegistry* registry{nullptr};
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
        ApplicationRegistry* registry,
        QObject *parent) :
    ApplicationController(parent)
{
    _d->app = app;
    _d->registry = registry;
}

ApplicationController::ApplicationController(QObject *parent) :
    IApplicationController(parent),
    _d(new ApplicationControllerPrivate(this))
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

    QString appExe = _d->app->meta()->applicationExecutablePath();
    if (!QFile::exists(appExe)) {
        WARN("Launch failed as requested application executable not exists");
        emit launchFailed();
        return;
    }

    DEBUG("Launching process: " << appExe);
    _d->process.setProgram(appExe);
    if (_d->registry) {
        // registry is used to identify who is making TCP connections
        QStringList args;
        args << "--application-code=" + _d->registry->createIdentificationCode(_d->app->meta()->applicationId());
        _d->process.setArguments(args);
    }

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
    }
}

void ApplicationController::resume()
{
    if (_d->process.state() == QProcess::Running)
    {
        int result = kill(_d->process.pid(), SIGCONT);
        DEBUG("Resumed " << _d->app->id() << ", got result " << result);
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
