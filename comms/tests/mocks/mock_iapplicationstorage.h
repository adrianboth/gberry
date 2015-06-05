#ifndef MOCK_IAPPLICATIONSTORAGE_H
#define MOCK_IAPPLICATIONSTORAGE_H

#include "interfaces/iapplicationsstorage.h"

#include <gmock/gmock.h>

class MockIApplicationsStorage : public IApplicationsStorage
{
    Q_OBJECT
public:
    MOCK_METHOD0(applications, QList<QSharedPointer<Application>>());

};

class StubIApplicationsStorage : public IApplicationsStorage
{
public:
    StubIApplicationsStorage(QList<QSharedPointer<Application>>& apps) : _apps(apps) {}

    virtual QList<QSharedPointer<Application>> applications() override { return _apps; }

    void emitApplicationUpdated() { emit IApplicationsStorage::applicationsUpdated(); }

private:
    QList<QSharedPointer<Application>>& _apps;
};

#endif // MOCK_IAPPLICATIONSTORAGE_H

