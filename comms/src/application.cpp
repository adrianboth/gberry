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

ApplicationMeta& Application::metaRef()
{
    // TODO: this ain't nicest thing
    QSharedPointer<ApplicationMeta> appMeta(qSharedPointerCast<ApplicationMeta>(meta()));
    return *appMeta.data();
}

QSharedPointer<Application> Application::copy(QSharedPointer<IApplication> iapp)
{
    ApplicationMeta* meta = new ApplicationMeta(*iapp->meta().data());
    QSharedPointer<Application> newApp(new Application(QSharedPointer<ApplicationMeta>(meta)));
    return newApp;
}
