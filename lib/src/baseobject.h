#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QString>

namespace GBerryLib {

/**
 * This is utility class to track objects, their creating, desctruction and usage.
 */
class BaseObject
{
public:
    explicit BaseObject(const QString& className = QString("default"));
    virtual ~BaseObject();

    int objectId() const;
    QString objectClassName() const;

private:
    int _objectId{-1};
    QString _objectClassName;

};

} // eon

#endif // BASEOBJECT_H
