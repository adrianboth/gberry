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

void LocalApplications::onApplicationsUpdated()
{
    _apps.clear();
    foreach(QSharedPointer<Application> app, _storage->applications()) {
        _apps[app->id()] = app;
    }
}
