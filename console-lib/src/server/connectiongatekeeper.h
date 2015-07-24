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
 
 #ifndef CONNECTIONGATEKEEPER_H
#define CONNECTIONGATEKEEPER_H

#include <QObject>
#include <QScopedPointer>

namespace GBerry {
namespace Console {
namespace Server {

class ConnectionGateKeeperPrivate;
class ApplicationRegistry;

/**
 * @brief The ConnectionGateKeeper class
 *
 * First a ping response and possible identification code is required from a
 * new connection.
 */
class ConnectionGateKeeper : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionGateKeeper(int connectionId, ApplicationRegistry* appRegistry, QObject *parent = 0);
    ~ConnectionGateKeeper();

    void validate();
    void incomingMessage(const QByteArray& msg);
    QString validatedApplicationId();

signals:
    void connectionValidated(int connectionId);
    void connectionDiscarded(int connectionId);

    void outgoingMessage(int connectionId, const QByteArray msg);

public slots:
    void waitTimeout();

private:
    const QScopedPointer<ConnectionGateKeeperPrivate> _d;
};

}}} // eon

#endif // CONNECTIONGATEKEEPER_H
