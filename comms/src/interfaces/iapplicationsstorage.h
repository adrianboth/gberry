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
