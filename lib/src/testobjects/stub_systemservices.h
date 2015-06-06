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
    TestSystemServices();
    ~TestSystemServices();

    // SystemServices
    virtual void singleshotTimer(int msec, QObject *receiver, const char *member);

    // TestSystemServices
    void invokeSingleshotTimer();
    void registerInstance();
    void unregisterInstance();

private:
    // TODO: later multiple
    QObject* _qobj;
    QString _signalName;

    bool _registered;
};

#endif // TESTSYSTEMSERVICES_H
