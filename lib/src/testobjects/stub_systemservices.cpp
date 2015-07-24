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
    _singleShotTimerWaitMs = msec;
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
