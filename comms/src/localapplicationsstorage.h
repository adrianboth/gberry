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

    // reads applications from local storage. Caller takes ownership
    virtual QList<QSharedPointer<Application> > applications() override;

    // TODO:
    QSharedPointer<LocalApplications> localApplications();

    // adds & writes application to local storage
    void addApplication(const Application& application);


signals:
    void applicationsUpdated();

public slots:

private:
    const QScopedPointer<LocalApplicationsStoragePrivate> _priv;

};

#endif // LOCALAPPLICATIONSSTORAGE_H
