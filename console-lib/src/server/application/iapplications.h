#ifndef IAPPLICATIONS_H
#define IAPPLICATIONS_H

#include <QSharedPointer>
#include <QString>
#include <QList>

#include <baseobject.h>
#include "iapplication.h"

#include <QDebug>

namespace GBerry {
    namespace Console {
        namespace Server {

class IApplications : public GBerryLib::BaseObject
{
public:
    explicit IApplications() : GBerryLib::BaseObject("IApplications") {}
    virtual ~IApplications() {}

public:
    // all applications but systemapps excluded
    virtual QList<QSharedPointer<IApplication>> applications() const = 0;

    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const = 0;

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const = 0;
};

}}} // eon

#endif // IAPPLICATIONS_H
