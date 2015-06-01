#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QPointer>

#include "iapplicationcontroller.h"
#include "applicationmeta.h"

class ApplicationController : public IApplicationController
{
    Q_OBJECT

public:
    explicit ApplicationController(QPointer<ApplicationMeta> meta, QObject *parent = 0);
    ApplicationController(QObject *parent = 0);
    ~ApplicationController();

    // IApplicationController
    virtual void launch();
    virtual void pause();
    virtual void resume();
    virtual void stop();

    // --
    virtual void setApplication(QPointer<ApplicationMeta> meta);
    virtual void enableSimulatedMode(bool enabled);

signals:

public slots:
    void onProcessFinished(int exitCode);
    void onProcessStateChanged(QProcess::ProcessState processState);

private:
    QPointer<ApplicationMeta> _appMeta;
    QProcess _process;

    enum CurrentAction { NONE, LAUNCHING, RESUMING, STOPPING };
    int _currentAction;
    bool _running;
    bool _simulated;
};


#endif // APPLICATIONCONTROLLER_H
