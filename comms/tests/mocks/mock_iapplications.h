#ifndef MOCK_IAPPLICATIONS_H
#define MOCK_IAPPLICATIONS_H

#include "server/application/iapplications.h"

class StubIApplications : public IApplications
{
public:
    StubIApplications(QMap<QString, QSharedPointer<IApplication>>& apps) : _apps(apps) {}

    virtual QList<QSharedPointer<IApplication>> applications() const override { return _apps.values(); }

    virtual QSharedPointer<IApplication> application(const QString& uniqueID) const override { return _apps[uniqueID]; }

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<IApplication>> applicationsByApplicationId(const QString& applicationId) const override {
        QList<QSharedPointer<IApplication>> found;
        foreach (auto app, _apps.values()) {
            if (app->meta()->applicationId() == applicationId)
                found << app;
        }

        return found;
    }

private:
    QMap<QString, QSharedPointer<IApplication>>& _apps;
};

#endif // MOCK_IAPPLICATIONS_H

