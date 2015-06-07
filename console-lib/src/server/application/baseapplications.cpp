#include "baseapplications.h"


namespace GBerry {
    namespace Console {
        namespace Server {

BaseApplications::BaseApplications()
{

}

BaseApplications::BaseApplications(QMap<QString, QSharedPointer<IApplication>>& apps) :
    _apps(apps)
{
}

BaseApplications::~BaseApplications()
{
}


QList<QSharedPointer<IApplication>> BaseApplications::applications() const
{
    return _apps.values();
}

QSharedPointer<IApplication> BaseApplications::application(const QString& uniqueID) const
{
    return _apps[uniqueID];
}

// there can be multiple versions for single application
QList<QSharedPointer<IApplication>> BaseApplications::applicationsByApplicationId(const QString& applicationId) const
{
    QList<QSharedPointer<IApplication>> found;
    foreach (auto app, _apps.values()) {
        if (app->meta()->applicationId() == applicationId)
            found << app;
    }

    return found;
}

void BaseApplications::add(QSharedPointer<IApplication> app)
{
    _apps[app->id()] = app;
}

}}} // eon
