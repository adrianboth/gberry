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
 
 #ifndef TESTSYSTEMSERVICES_H
#define TESTSYSTEMSERVICES_H

#include "systemservices.h"

/**
 * @brief Test object for SystemServices class.
 *
 * Usage:
 *
 *   TestSystemServices* testservices = new TestSystemServices();
 *   QVariant var = QVariant::fromValue(qobject_cast<QObject*> (testservices));
 *   QCoreApplication::instance()->setProperty(SystemServices::SERVICE_NAME, var);
 *
 * Or simply:
 *   QScopedPointer<TestSystemServices> testservices(new TestSystemServices);
 *   testservices->registerInstance();
 */
class TestSystemServices : public SystemServices
{
public:
    explicit TestSystemServices();
    virtual ~TestSystemServices();

    // SystemServices
    virtual void singleshotTimer(int msec, QObject *receiver, const char *member) override;

    // TestSystemServices
    void invokeSingleshotTimer();
    void registerInstance();
    void unregisterInstance();

    // for singleShotTimer
    int singleShotTimerWaitMs() const { return _singleShotTimerWaitMs; }

private:
    // TODO: later multiple
    QObject* _qobj;
    QString _signalName;
    int _singleShotTimerWaitMs{0};

    bool _registered;
};

#endif // TESTSYSTEMSERVICES_H
