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
 
 #ifndef LOCALAPPLICATIONS_H
#define LOCALAPPLICATIONS_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "server/application/baseapplications.h"
#include "interfaces/iapplicationsstorage.h"

using namespace GBerry::Console::Server;

class LocalApplications : public QObject, public BaseApplications
{
    Q_OBJECT
public:
    explicit LocalApplications(IApplicationsStorage* storage, QObject *parent = 0);
    virtual ~LocalApplications();

signals:

public slots:

private slots:
    void onApplicationsUpdated();

private:
    IApplicationsStorage* _storage;

};

#endif // LOCALAPPLICATIONS_H
