#ifndef LOCALAPPLICATIONSSTORAGE_H
#define LOCALAPPLICATIONSSTORAGE_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QDir>

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


signals:
    void applicationsUpdated();

public slots:

private:
    LocalApplicationsStoragePrivate* _priv;

};

#endif // LOCALAPPLICATIONSSTORAGE_H
