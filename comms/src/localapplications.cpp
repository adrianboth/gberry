#include "localapplications.h"

LocalApplications::LocalApplications(IApplicationsStorage* storage, QObject *parent) :
    QObject(parent),
    _storage(storage)
{
    // read applications first time
    onApplicationsUpdated();

    connect(storage, &IApplicationsStorage::applicationsUpdated,
            this, &LocalApplications::onApplicationsUpdated);
}

LocalApplications::~LocalApplications()
{

}

QList<QSharedPointer<ApplicationMeta>> LocalApplications::applications() const
{
    return _apps.values();
}

QList<QSharedPointer<ApplicationMeta>> LocalApplications::applicationsByApplicationId(const QString& applicationId) const
{
    QList<QSharedPointer<ApplicationMeta>> found;
    foreach (auto appmeta, _apps) {
       if (appmeta->applicationId() == applicationId) {
           found << appmeta;
       }
    }

    return found;
}

QSharedPointer<ApplicationMeta> LocalApplications::application(const QString& uniqueID) const
{
    // TODO: what if not found
    return _apps[uniqueID];
}

void LocalApplications::onApplicationsUpdated()
{
    _apps.clear();
    foreach(QSharedPointer<Application> app, _storage->applications()) {
        QSharedPointer<ApplicationMeta> p(app->meta());
        _apps[app->id()] = p;
    }
}
