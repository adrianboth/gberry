#ifndef SYSTEMSERVICES_H
#define SYSTEMSERVICES_H

#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include <QProcessEnvironment>



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



class IEnvironmentVariables
{

public:
    IEnvironmentVariables() {}
    virtual ~IEnvironmentVariables() {}

    virtual bool contains(const QString& key) const = 0;
    virtual QString value(const QString& key, const QString& defaultValue = QString()) const = 0;
};


class EnvironmentVariables : public IEnvironmentVariables
{
public:
    EnvironmentVariables(QProcessEnvironment env);
    virtual ~EnvironmentVariables();

    virtual bool contains(const QString& key) const override;
    virtual QString value(const QString& key, const QString& defaultValue = QString()) const override;

private:
    QProcessEnvironment _env;
};

#endif // SYSTEMSERVICES_H

