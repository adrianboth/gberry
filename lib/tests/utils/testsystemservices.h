#ifndef TESTSYSTEMSERVICES_H
#define TESTSYSTEMSERVICES_H

#include "systemservices.h"

class TestSystemServices : public SystemServices
{
public:
    TestSystemServices();
    ~TestSystemServices();

    virtual void singleshotTimer(int msec, QObject *receiver, const char *member);

    void invokeSingleshotTimer();

private:
    // TODO: later multiple
    QObject* _qobj;
    QString _signalName;
};

#endif // TESTSYSTEMSERVICES_H
