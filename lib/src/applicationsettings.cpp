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
