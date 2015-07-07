#include "localapplicationsstorage.h"

#include <QSharedPointer>
#include <QDir>

#include "applicationconfigreaderwriter.h"
#include "localapplications.h"

#define LOG_AREA "LocalApplicationsStorage"
#include "log/log.h"


class LocalApplicationsStoragePrivate
{
public:
    LocalApplicationsStoragePrivate() {}

    QDir appsDir;
};

LocalApplicationsStorage::LocalApplicationsStorage(QString appsDir, QObject *parent) :
    IApplicationsStorage(parent),
    _priv(new LocalApplicationsStoragePrivate)
{
    _priv->appsDir = appsDir;

    if (!_priv->appsDir.exists()) {
        WARN("Apps directory doesn't exist");
    }
}

LocalApplicationsStorage::~LocalApplicationsStorage()
{
}

QList<QSharedPointer<Application>> LocalApplicationsStorage::applications()
{
    QList<QSharedPointer<Application>> apps;

    // iterate folders in apps dir
    // read *appcfg.json from each folder

    QStringList dirs = _priv->appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(QString dirName, dirs) {
        ApplicationConfigReaderWriter reader(_priv->appsDir.filePath(dirName));

        QSharedPointer<Application> app(reader.readApplication());
        if (!app.isNull()) {
            DEBUG("Application" << app->id() << "read successfully");
            apps.append(app);
        }
    }

    return apps;
}

QSharedPointer<LocalApplications> LocalApplicationsStorage::localApplications()
{
    return QSharedPointer<LocalApplications>(new LocalApplications(this));
}

void LocalApplicationsStorage::addApplication(const Application &application)
{
    // TODO:
}
