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

bool LocalApplicationsStorage::updateApplication(const Application &application, LocalApplicationsStorage::Result &result)
{
    // TODO:
    QSharedPointer<LocalApplications> applications(localApplications());
    QSharedPointer<IApplication> existingApp = applications->application(application.id());

    if (existingApp.isNull()) {
        // application doesn't exists -> can't be updated
        return result.record(Result::UpdateErrorApplicationNotExists, QString("Can't add update because application with id %1 does not exists.").arg(application.id()));
    }

    ApplicationConfigReaderWriter writer(existingApp->meta()->applicationDirPath());

    ApplicationConfigReaderWriter::Result writerResult;
    if (writer.writeApplication(application, writerResult)) {
        emit applicationsUpdated();
        return true;
    } else {
        result.record(Result::ApplicationConfigurationWritingFailed, QString("Failed to write application configuration to disk: %1").arg(writerResult.errorString));
        return false;
    }
}

bool LocalApplicationsStorage::addApplication(Application &application, Result& result)
{
    // TODO:
    QSharedPointer<LocalApplications> applications(localApplications());

    QSharedPointer<IApplication> existingApp = applications->application(application.id());

    if (!existingApp.isNull()) {
        // there exists already app with same name ...
        return result.record(Result::ApplicationExists, QString("Can't add application because application with id %1 already exists.").arg(application.id()));
    }

    // folder might still exist but it is not valid application

    QDir appDir(_priv->appsDir.filePath(application.id()));
    if (appDir.exists()) {
        // TODO: at least now in first place we fail if dir exists but maybe later we have better strategy
        return result.record(Result::ApplicationDirExists, QString("Application directory %1 already exists").arg(appDir.path()));
    }

    if (!appDir.mkpath(appDir.absolutePath())) {
        return result.record(Result::FailedToCreateApplicationDirectory, QString("Failed to create application directory %1").arg(appDir.path()));
    }

    ApplicationMeta& metaRef = application.metaRef();
    metaRef.setApplicationDirPath(appDir.absolutePath());

    ApplicationConfigReaderWriter writer(appDir.path());

    ApplicationConfigReaderWriter::Result writerResult;
    if (writer.writeApplication(application, writerResult)) {
        emit applicationsUpdated();
        return true;
    } else {
        result.record(Result::ApplicationConfigurationWritingFailed, QString("Failed to write application configuration to disk: %1").arg(writerResult.errorString));
        return false;
    }

}
