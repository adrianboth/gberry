#ifndef IAPPLICATIONSSTORAGE_H
#define IAPPLICATIONSSTORAGE_H

#include <QObject>
#include <QSharedPointer>

#include "application.h"

class IApplicationsStorage : public QObject
{
    Q_OBJECT

public:
    explicit IApplicationsStorage(QObject* parent = 0) : QObject(parent) {}
    ~IApplicationsStorage() {}

    // reads applications from local storage. Caller takes ownership
    virtual QList<QSharedPointer<Application>> applications() = 0;

signals:
    void applicationsUpdated();
};

#endif // IAPPLICATIONSSTORAGE_H
