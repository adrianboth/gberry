#ifndef IAPPLICATIONS
#define IAPPLICATIONS

#include <QSharedPointer>
#include <QString>
#include <QList>

#include "applicationmeta.h"

class IApplications
{
public:
    virtual QList<QSharedPointer<ApplicationMeta>> applications() const = 0;

    virtual QSharedPointer<ApplicationMeta> application(const QString& uniqueID) const = 0;

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<ApplicationMeta>> applicationsByApplicationId(const QString& applicationId) const = 0;
};

#endif // IAPPLICATIONS

