#ifndef IAPPLICATIONS
#define IAPPLICATIONS

#include <QPointer>
#include <QString>
#include <QList>

#include "applicationmeta.h"

class IApplications
{
public:
    virtual QList<QPointer<ApplicationMeta>> applications() const = 0;
    virtual QPointer<ApplicationMeta> application(QString appID) const = 0;
};

#endif // IAPPLICATIONS

