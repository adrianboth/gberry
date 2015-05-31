#ifndef LOCALAPPLICATIONSSTORAGE_H
#define LOCALAPPLICATIONSSTORAGE_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "applicationmeta.h"

class LocalApplicationsStorage : public QObject
{
    Q_OBJECT
public:
    explicit LocalApplicationsStorage(QObject *parent = 0);
    ~LocalApplicationsStorage();

    // reads applications from local storage. Caller takes ownership
    virtual QList<QPointer<ApplicationMeta>> applications();

signals:
    void applicationsUpdated();

public slots:
};

#endif // LOCALAPPLICATIONSSTORAGE_H
