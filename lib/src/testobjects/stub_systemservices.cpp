#include "stub_systemservices.h"

#include <QDebug>
#include <QCoreApplication>

TestSystemServices::TestSystemServices() : _qobj(NULL), _registered(false)
{

}

TestSystemServices::~TestSystemServices()
{
    unregisterInstance();
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

void TestSystemServices::registerInstance()
{
    QVariant var = QVariant::fromValue(qobject_cast<QObject*> (this));
    QCoreApplication::instance()->setProperty(SystemServices::SERVICE_NAME, var);
    _registered = true;
}

void TestSystemServices::unregisterInstance()
{
    QVariant obj = QCoreApplication::instance()->property(SystemServices::SERVICE_NAME);
    if (_registered && obj.isValid()) {
        QCoreApplication::instance()->setProperty(SystemServices::SERVICE_NAME, QVariant());
        _registered = false;
    }
}
