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
 
 #include "applicationsettings.h"
#include <QDebug>

ApplicationSettings::ApplicationSettings(QObject* parent) :
    QObject(parent),
    _runningOnTargetDevice(false)
{
#ifdef TARGET_DESKTOP
    _runningOnTargetDevice = false;
    qDebug() << "[ApplicationSettings] Running on Desktop";
#else
    _runningOnTargetDevice = true;
    qDebug() << "[ApplicationSettings] Running on target hardware";
#endif
}

ApplicationSettings::~ApplicationSettings()
{

}

void ApplicationSettings::setRunningOnTargetDevice(bool b)
{
    if (_runningOnTargetDevice != b) {
        _runningOnTargetDevice = b;
        emit runningOnTargetDeviceChanged();
    }
}
