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
