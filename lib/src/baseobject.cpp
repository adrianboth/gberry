#include "baseobject.h"

#define LOG_AREA "BaseObject"
#include "log/log.h"

namespace GBerryLib {

BaseObject::BaseObject(const QString& className) :
    _objectClassName(className)
{
    static int objectCounter = 0;
    _objectId = ++objectCounter;
    DEBUG("Constructor:" << _objectClassName << "-" << _objectId);
}

BaseObject::~BaseObject()
{
    TRACE("Desctructor:" << _objectClassName << "-" << _objectId);
}

int BaseObject::objectId() const
{
    return _objectId;
}

QString BaseObject::objectClassName() const
{
    return _objectClassName;
}

} // eon
