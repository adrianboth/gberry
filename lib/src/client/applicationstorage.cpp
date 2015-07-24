/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
