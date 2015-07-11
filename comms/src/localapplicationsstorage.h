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
            ApplicationDirExists,
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

signals:
    void applicationsUpdated();

public slots:

private:
    const QScopedPointer<LocalApplicationsStoragePrivate> _priv;

};

#endif // LOCALAPPLICATIONSSTORAGE_H
