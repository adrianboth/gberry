#include "application.h"

#include <QDebug>

Application::Application(QSharedPointer<ApplicationMeta> meta) :
    _meta(meta)
{

}

Application::~Application()
{
    //qDebug() << "[Application] desctructor";
}

QString Application::id() const
{
    return _meta->id();
}

Application::State Application::state() const
{
    return _state;
}

void Application::markState(Application::State newState)
{
    _state = newState;
}

QSharedPointer<ApplicationMeta> Application::meta() const
{
    return _meta;
}
