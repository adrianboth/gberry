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

QList<QPointer<ApplicationMeta>> LocalApplications::applications() const
{
    return _apps.values();
}

QPointer<ApplicationMeta> LocalApplications::application(QString appID) const
{
    // TODO: what if not found
    return _apps[appID];
}

void LocalApplications::onApplicationsUpdated()
{
    _apps.clear();
    foreach(auto meta, _storage->applications()) {
        _apps[meta->id()] = meta;
    }
}
