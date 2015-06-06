#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QPointer>

#include "interfaces/iapplicationcontroller.h"
#include "applicationmeta.h"

class ApplicationController : public IApplicationController
{
    Q_OBJECT

public:
    explicit ApplicationController(QSharedPointer<IApplicationMeta> meta, QObject *parent = 0);
    ApplicationController(QObject *parent = 0);
    ~ApplicationController();

    // IApplicationController
    virtual void launch();
    virtual void pause();
    virtual void resume();
    virtual void stop();

    // --
    virtual void setApplication(QSharedPointer<ApplicationMeta> meta);
    virtual void enableSimulatedMode(bool enabled);

    // -- dynamic properties
    static const char* PROCESS_KILL_WAIT_MS_PROP;

signals:

public slots:
    void onProcessFinished(int exitCode);
    void onProcessStateChanged(QProcess::ProcessState processState);

    // relaunch is done if original launch() call need to be delayed
    void relaunchAfterDelay();

private:
    QSharedPointer<IApplicationMeta> _appMeta;
    QProcess _process;

    enum CurrentAction { NONE, LAUNCHING, RESUMING, STOPPING };
    int _currentAction;
    bool _running;
    bool _simulated;
    int _timerForWaitingProcessToStopRunning;
};


#endif // APPLICATIONCONTROLLER_H
