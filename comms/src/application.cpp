#include "application.h"

#include <QDebug>

Application::Application(QSharedPointer<ApplicationMeta> meta) :
    BaseApplication(meta)
{
}

Application::~Application()
{
    //qDebug() << "[Application] desctructor";
}
