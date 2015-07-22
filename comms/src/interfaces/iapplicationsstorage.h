#ifndef IAPPLICATIONSSTORAGE_H
#define IAPPLICATIONSSTORAGE_H

#include <QObject>
#include <QDebug>
#include <QSharedPointer>

#include "application.h"

class IApplicationsStorage : public QObject
{
    Q_OBJECT

public:
    explicit IApplicationsStorage(QObject* parent = 0) : QObject(parent) {

        static int counter = 0;
        objId = ++counter;
        qDebug() << "--- IApplicationsStorage " << objId;
    }
    ~IApplicationsStorage() { qDebug() << "--- IApplicationsStorage DESC " << objId;}

    int objId;
    int objectId() const { return objId; }

    // reads applications from local storage. Caller takes ownership
    virtual QList<QSharedPointer<Application>> applications() = 0;

signals:
    void applicationsUpdated();
};

#endif // IAPPLICATIONSSTORAGE_H
