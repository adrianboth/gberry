#include "systemservices.h"

const char* SystemServices::SERVICE_NAME = "systemservices";

SystemServices* SystemServices::__defaultInstance = new SystemServices();


SystemServices* SystemServices::instance()
{
    QCoreApplication* app = QCoreApplication::instance();
    QVariant var = app->property(SERVICE_NAME);
    if (var.isValid() && var.canConvert<QObject*>())
    {
        QObject* obj = var.value<QObject*>();
        SystemServices* services = qobject_cast<SystemServices*>(obj);
        return services;
    }
    else
    {
        qWarning("WARNING: Couldn't find valid SystemServices");
        return __defaultInstance;
    }
    return NULL;
}

// ---

EnvironmentVariables::EnvironmentVariables(const QProcessEnvironment env) :
    _env(env)
{
}

EnvironmentVariables::~EnvironmentVariables()
{
}

bool EnvironmentVariables::contains(const QString &key) const
{
    return _env.contains(key);
}

QString EnvironmentVariables::value(const QString &key, const QString &defaultValue) const
{
    return _env.value(key, defaultValue);
}
