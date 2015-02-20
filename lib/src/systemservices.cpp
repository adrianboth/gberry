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
