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

