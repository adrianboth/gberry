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
 
 #ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>

class ApplicationSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool runningOnTargetDevice READ runningOnTargetDevice NOTIFY runningOnTargetDeviceChanged)
public:
    ApplicationSettings(QObject* parent = 0);
    ~ApplicationSettings();

    bool runningOnTargetDevice() const { return _runningOnTargetDevice; }


    void setRunningOnTargetDevice(bool b);

signals:
    void runningOnTargetDeviceChanged();

private:
    bool _runningOnTargetDevice;
};

#endif // APPLICATIONSETTINGS_H
