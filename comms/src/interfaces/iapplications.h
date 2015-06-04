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
    virtual QSharedPointer<ApplicationMeta> application(QString appID) const = 0;
};

#endif // IAPPLICATIONS

