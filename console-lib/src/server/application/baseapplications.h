#ifndef BASEAPPLICATIONS_H
#define BASEAPPLICATIONS_H

#include <QMap>

#include "server/application/iapplications.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class BaseApplications : public IApplications
{
public:
    explicit BaseApplications();
    explicit BaseApplications(QMap<QString, QSharedPointer<IApplication>>& apps);
    virtual ~BaseApplications();

    // -- IApplications
    virtual QList<QSharedPointer<IApplication>> applications() const override;
    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const override;
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const override;

    void add(QSharedPointer<IApplication> app);

protected:
    QMap<QString, QSharedPointer<IApplication>> _apps;

};

}}} // eon

#endif // BASEAPPLICATIONS_H
