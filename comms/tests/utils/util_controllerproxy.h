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
    virtual void launch() override { if (_target) _target->launch(); }
    virtual void pause() override { if (_target) _target->pause(); }
    virtual void resume() override { if (_target) _target->resume(); }
    virtual void stop() override { if (_target) _target->stop(); }
    virtual QString fullApplicationId() const override {
        if (_target)
            return _target->fullApplicationId();
        else
            return "";
    }

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
    virtual void launch() override { if (_target) _target->launch(); }
    virtual void pause() override { if (_target) _target->pause(); }
    virtual void resume() override { if (_target) _target->resume(); }
    virtual void stop() override { if (_target) _target->stop(); }

    virtual QString fullApplicationId() const override {
        if (_target)
            return _target->fullApplicationId();
        else
            return "";
    }

    virtual bool useApplication(const QString& appID) override {
        if (_target)
            return _target->useApplication(appID);
        else
            return false;
    }

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

