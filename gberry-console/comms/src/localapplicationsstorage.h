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
 
 #ifndef LOCALAPPLICATIONSSTORAGE_H
#define LOCALAPPLICATIONSSTORAGE_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QDir>

#include "localapplications.h"
#include "interfaces/iapplicationsstorage.h"

class LocalApplicationsStoragePrivate;

class LocalApplicationsStorage : public IApplicationsStorage
{
    Q_OBJECT

public:
    explicit LocalApplicationsStorage(QString rootAppsDir, QObject *parent = 0);
    ~LocalApplicationsStorage();

    class Result {
    public:
        enum Error {
            NoError,
            ApplicationExists,
            ApplicationNotExists,
            ApplicationDirExists,
            ApplicationDeletionFailed,
            FailedToCreateApplicationDirectory,
            ApplicationConfigurationWritingFailed,
            UpdateErrorApplicationNotExists, // TODO: make diff between update and add errors
        };

        Error error{NoError};
        // textual information for developer, not to be localized as such
        QString errorString;

        bool hasError() const { return error != NoError; }

        // returns false if there are errors
        bool record(Error error_, const QString& errorString_) {
            error = error_;
            errorString = errorString_;
            return !hasError();
        }

    };


    // reads applications from local storage. Caller takes ownership
    virtual QList<QSharedPointer<Application> > applications() override;

    // TODO:
    QSharedPointer<LocalApplications> localApplications();

    // adds & writes application to local storage
    bool addApplication(Application& application, Result& result);

    bool updateApplication(const Application& application, Result& result);

    // cleans up possible temp files
    bool pruneApplication(QSharedPointer<IApplication> application);

    bool deleteApplication(const Application& application, Result& result);

signals:
    void applicationsUpdated();

public slots:

private:
    const QScopedPointer<LocalApplicationsStoragePrivate> _d;

};

#endif // LOCALAPPLICATIONSSTORAGE_H
