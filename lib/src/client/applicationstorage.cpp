#include "applicationstorage.h"

#include <QProcessEnvironment>
#include <QDir>
#include <QDebug>

ApplicationStorage::ApplicationStorage(QQmlApplicationEngine* engine) :
    _engine(engine)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // default path
    QDir storageDir(_engine->offlineStoragePath());

    if (!storageDir.exists()) {
        if (!storageDir.mkpath(".")) {
            // TODO: we should stop whole application initialization here
            qCritical() << "ERROR: Failed to create application storage directory:" << storageDir.path();
            return;
        }
    }

    if (env.contains("MOBAPP_ID")) {
        QString appID = env.value("MOBAPP_ID");
        QString subdir("app_" + appID);
        if (!storageDir.mkpath(subdir)) {
            qCritical() << "ERROR: Failed to create subdirectory:" << subdir << "to" << storageDir.path();
            return;
        }
        storageDir.cd(subdir);
        _engine->setOfflineStoragePath(storageDir.path());
    }

    qDebug() << "QQmlEngine::offlineStoragePath()" << engine->offlineStoragePath();
}

ApplicationStorage::~ApplicationStorage()
{

}

QString ApplicationStorage::storagePath() const {
    return _engine->offlineStoragePath();
}
