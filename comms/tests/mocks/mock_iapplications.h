#ifndef MOCK_IAPPLICATIONS_H
#define MOCK_IAPPLICATIONS_H

#include "interfaces/iapplicationmetas.h"

class StubIApplications : public IApplicationMetas
{
public:
    StubIApplications(QMap<QString, QSharedPointer<ApplicationMeta>>& apps) : _apps(apps) {}

    virtual QList<QSharedPointer<ApplicationMeta>> applications() const override { return _apps.values(); }

    virtual QSharedPointer<ApplicationMeta> application(const QString& uniqueID) const override { return _apps[uniqueID]; }

    // there can be multiple versions for single application
    virtual QList<QSharedPointer<ApplicationMeta>> applicationsByApplicationId(const QString& applicationId) const override {
        QList<QSharedPointer<ApplicationMeta>> found;
        foreach (auto app, _apps.values()) {
            if (app->applicationId() == applicationId)
                found << app;
        }

        return found;
    }

private:
    QMap<QString, QSharedPointer<ApplicationMeta>>& _apps;
};

#endif // MOCK_IAPPLICATIONS_H

