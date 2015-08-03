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
#ifndef SERVERCONNECTIONMODEL_H
#define SERVERCONNECTIONMODEL_H

#include <QObject>
#include <QScopedPointer>

class InvocationFactory;

namespace GBerryClient {

// TODO: ?? would be better to move whole settings model to C++ side ??
class ServerConnectionModel : public QObject
{
    Q_OBJECT
public:
    explicit ServerConnectionModel(InvocationFactory* invocationFactory, QObject *parent = 0);
    ~ServerConnectionModel();

    Q_INVOKABLE void setServerHost(const QString& host);

    // TODO: ping stuff

signals:

public slots:

private:
    class Private;
    const QScopedPointer<Private> _d;
};

} // eon

#endif // SERVERCONNECTIONMODEL_H
