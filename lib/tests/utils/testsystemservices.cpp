#include "testsystemservices.h"

#include <QDebug>

TestSystemServices::TestSystemServices() : _qobj(NULL)
{

}

TestSystemServices::~TestSystemServices()
{

}

void TestSystemServices::singleshotTimer(int msec, QObject *receiver, const char *member)
{
    Q_UNUSED(msec);
    _qobj = receiver;
    _signalName = member;
    _signalName.remove(0, 1);
    _signalName.remove(_signalName.length() - 2, 2);
}

void TestSystemServices::invokeSingleshotTimer()
{
    if (_qobj)
    {
        qDebug() << "SIGNAL NAME: " << _signalName;
        qDebug() << "SIGNAL NAME: " << qPrintable(_signalName);
        _qobj->metaObject()->invokeMethod(_qobj, qPrintable(_signalName));
    }
}
