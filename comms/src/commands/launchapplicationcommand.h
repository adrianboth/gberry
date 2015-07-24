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
 
 #ifndef LAUNCHAPPLICATIONCOMMAND_H
#define LAUNCHAPPLICATIONCOMMAND_H

#include <QObject>
#include <QJsonObject>

#include <server/icommand.h>
#include <server/application/iapplications.h>
using namespace GBerry::Console::Server;

namespace GBerryComms {

class LaunchApplicationCommandPrivate;

class LaunchApplicationCommand : public QObject, public ICommand
{
    Q_OBJECT
public:
    // TODO: how to create factory like
    LaunchApplicationCommand(QSharedPointer<IApplications> apps);
    ~LaunchApplicationCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

signals:
    void launchApplicationRequested(const QString applicationId);

private:
    LaunchApplicationCommandPrivate* _priv;

};

} // eon


#endif // LAUNCHAPPLICATIONCOMMAND_H
