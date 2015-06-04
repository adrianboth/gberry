#include "localapplications.h"

LocalApplications::LocalApplications(LocalApplicationsStorage* storage, QObject *parent) :
    QObject(parent),
    _storage(storage)
{
    // read applications first time
    onApplicationsUpdated();

    connect(storage, &LocalApplicationsStorage::applicationsUpdated,
            this, &LocalApplications::onApplicationsUpdated);
}

LocalApplications::~LocalApplications()
{

}

QList<QSharedPointer<ApplicationMeta>> LocalApplications::applications() const
{
    return _apps.values();
}

QSharedPointer<ApplicationMeta> LocalApplications::application(QString appID) const
{
    // TODO: what if not found
    return _apps[appID];
}

void LocalApplications::onApplicationsUpdated()
{
    _apps.clear();
    foreach(QSharedPointer<Application> app, _storage->applications()) {
        QSharedPointer<ApplicationMeta> p(app->meta());
        _apps[app->id()] = p;
    }
}
