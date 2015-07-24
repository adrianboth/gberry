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
 
 #include "launchapplicationcommand.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "server/application/application2json.h"

#define LOG_AREA "LaunchApplicationCommand"
#include "log/log.h"

namespace GBerryComms {


class LaunchApplicationCommandPrivate
{
public:
    LaunchApplicationCommandPrivate(QSharedPointer<IApplications> apps_) :
        apps(apps_) {}

    QSharedPointer<IApplications> apps;
};

LaunchApplicationCommand::LaunchApplicationCommand(QSharedPointer<IApplications> apps) :
    ICommand("LaunchApplication")
{
    _priv = new LaunchApplicationCommandPrivate(apps);
}

LaunchApplicationCommand::~LaunchApplicationCommand()
{
    delete _priv;
}

bool LaunchApplicationCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no response message
    Q_UNUSED(response);

    if (json.contains("application_id")) {
        emit launchApplicationRequested(json["application_id"].toString());
        return true;
    } else {
        WARN("Invalid LaunchApplicationCommand: application_id missing");
        return false;
    }
}

} // eon
