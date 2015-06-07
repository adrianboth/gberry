#include "baseapplication.h"

#include <QDebug>

namespace GBerry {
    namespace Console {
        namespace Server {

BaseApplication::BaseApplication(QSharedPointer<ApplicationMeta> meta) :
    _meta(meta),
    _state(IApplication::Valid)
{

}

BaseApplication::~BaseApplication()
{
    //qDebug() << "[BaseApplication] desctructor";
}

QString BaseApplication::id() const
{
    return _meta->id();
}

BaseApplication::State BaseApplication::state() const
{
    return _state;
}

void BaseApplication::markState(BaseApplication::State newState)
{
    _state = newState;
}

QSharedPointer<IApplicationMeta> BaseApplication::meta() const
{
    return _meta;
}

}}} // eon
