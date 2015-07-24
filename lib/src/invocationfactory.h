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
 
 #ifndef RESTINVOCATIONFACTORY_H
#define RESTINVOCATIONFACTORY_H

#include <QObject>
#include <QNetworkAccessManager>

class RESTInvocation;

namespace GBerry {
    class DownloadStreamInvocation;
}
using namespace GBerry;

// TODO: namespace
// TODO: I prefix (then *.cpp file could be removed

class InvocationFactory : public QObject
{
    Q_OBJECT
public:
    explicit InvocationFactory(QObject *parent = 0);
    virtual ~InvocationFactory();

    virtual RESTInvocation* newRESTInvocation() = 0;
    virtual DownloadStreamInvocation* newDownloadStreamInvocation() = 0;
    virtual QString defaultHostName() const = 0;

signals:

public slots:
};

#endif // RESTINVOCATIONFACTORY_H
