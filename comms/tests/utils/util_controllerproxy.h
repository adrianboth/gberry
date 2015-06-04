#ifndef UTIL_CONTROLLERPROXY_H
#define UTIL_CONTROLLERPROXY_H

#include <QPointer>

#include "interfaces/iapplicationcontroller.h"
#include "interfaces/ilaunchcontroller.h"

class ProxyIApplicationController : public IApplicationController
{
    Q_OBJECT

public:
    ProxyIApplicationController(QPointer<IApplicationController> c, QObject* parent = 0) :
        IApplicationController(parent),
        _target(NULL)
        { setTarget(c); }
    ~ProxyIApplicationController() {}

    // IApplicationController
    virtual void launch() { if (_target) _target->launch(); }
    virtual void pause() { if (_target) _target->pause(); }
    virtual void resume() { if (_target) _target->resume(); }
    virtual void stop() { if (_target) _target->stop(); }

    // --
    void setTarget(QPointer<IApplicationController> c)
    {
        if (_target) {

            disconnect(_target.data(), &IApplicationController::launched, this, &ProxyIApplicationController::onTargetLaunched);
            disconnect(_target.data(), &IApplicationController::launchFailed, this, &ProxyIApplicationController::onTargetLaunchFailed);
            disconnect(_target.data(), &IApplicationController::resumed, this, &ProxyIApplicationController::onTargetResumed);
            disconnect(_target.data(), &IApplicationController::resumeFailed, this, &ProxyIApplicationController::onTargetResumeFailed);
            disconnect(_target.data(), &IApplicationController::died, this, &ProxyIApplicationController::onTargetDied);
            _target.clear();
        }

        if (c) {
            _target = c;
            connect(_target.data(), &IApplicationController::launched, this, &ProxyIApplicationController::onTargetLaunched);
            connect(_target.data(), &IApplicationController::launchFailed, this, &ProxyIApplicationController::onTargetLaunchFailed);
            connect(_target.data(), &IApplicationController::resumed, this, &ProxyIApplicationController::onTargetResumed);
            connect(_target.data(), &IApplicationController::resumeFailed, this, &ProxyIApplicationController::onTargetResumeFailed);
            connect(_target.data(), &IApplicationController::died, this, &ProxyIApplicationController::onTargetDied);
        }
    }

signals:
    void launched();
    void launchFailed();
    void resumed();
    void resumeFailed();
    void died();

private:
    QPointer<IApplicationController> _target;

public slots:

    void onTargetLaunched() { emit launched(); }
    void onTargetLaunchFailed() { emit launchFailed(); }
    void onTargetResumed() { emit resumed(); }
    void onTargetResumeFailed() { emit resumeFailed(); }
    void onTargetDied() { emit died(); }
};


class ProxyILaunchController : public ILaunchController
{
    Q_OBJECT

public:
    ProxyILaunchController(QPointer<ILaunchController> c, QObject* parent = 0) :
        ILaunchController(parent),
        _target(NULL)
        { setTarget(c); }
    ~ProxyILaunchController() {}

    // ILaunchController
    virtual void launch() { if (_target) _target->launch(); }
    virtual void pause() { if (_target) _target->pause(); }
    virtual void resume() { if (_target) _target->resume(); }
    virtual void stop() { if (_target) _target->stop(); }

    virtual void useApplication(QString& appID) { if (_target) _target->useApplication(appID); }

    // --
    void setTarget(QPointer<ILaunchController> c)
    {
        if (_target) {

            disconnect(_target.data(), &ILaunchController::launched, this, &ProxyILaunchController::onTargetLaunched);
            disconnect(_target.data(), &ILaunchController::launchFailed, this, &ProxyILaunchController::onTargetLaunchFailed);
            disconnect(_target.data(), &ILaunchController::resumed, this, &ProxyILaunchController::onTargetResumed);
            disconnect(_target.data(), &ILaunchController::resumeFailed, this, &ProxyILaunchController::onTargetResumeFailed);
            disconnect(_target.data(), &ILaunchController::died, this, &ProxyILaunchController::onTargetDied);
            _target.clear();
        }

        if (c) {
            _target = c;
            connect(_target.data(), &ILaunchController::launched, this, &ProxyILaunchController::onTargetLaunched);
            connect(_target.data(), &ILaunchController::launchFailed, this, &ProxyILaunchController::onTargetLaunchFailed);
            connect(_target.data(), &ILaunchController::resumed, this, &ProxyILaunchController::onTargetResumed);
            connect(_target.data(), &ILaunchController::resumeFailed, this, &ProxyILaunchController::onTargetResumeFailed);
            connect(_target.data(), &ILaunchController::died, this, &ProxyILaunchController::onTargetDied);
        }
    }

signals:
    void launched();
    void launchFailed();
    void resumed();
    void resumeFailed();
    void died();

private:
    QPointer<ILaunchController> _target;

private slots:

    void onTargetLaunched() { emit launched(); }
    void onTargetLaunchFailed() { emit launchFailed(); }
    void onTargetResumed() { emit resumed(); }
    void onTargetResumeFailed() { emit resumeFailed(); }
    void onTargetDied() { emit died(); }
};

#endif // UTIL_CONTROLLERPROXY_H

