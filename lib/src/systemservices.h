#ifndef SYSTEMSERVICES_H
#define SYSTEMSERVICES_H

#include <QObject>
#include <QCoreApplication>
#include <QVariant>



class SystemServices : public QObject
{
    Q_OBJECT

public:
    static const char* SERVICE_NAME;

    virtual void singleshotTimer(int msec, QObject *receiver, const char *member)
    {
        Q_UNUSED(msec); Q_UNUSED(receiver); Q_UNUSED(member);
        qWarning("WARNING: Current instance of SystemServices doesn't implement singleshotTimer()");
    }

    static SystemServices * instance();

private:
    static SystemServices* __defaultInstance;
};

//const char* SystemServices::SERVICE_NAME = "systemservices";

#endif // SYSTEMSERVICES_H

