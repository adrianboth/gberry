#include "realsystemservices.h"

#include <QTimer>

RealSystemServices::RealSystemServices()
{

}

RealSystemServices::~RealSystemServices()
{

}

void RealSystemServices::singleshotTimer(int msec, QObject *receiver, const char *member)
{
    QTimer::singleShot(msec, receiver, member);
}

void RealSystemServices::registerItself()
{
    QCoreApplication* app = QCoreApplication::instance();
    QObject *obj = this;
    QVariant var = QVariant::fromValue(obj);

    // note that we are not freeing previous service anyway
    app->setProperty("systemservices", var);
}
