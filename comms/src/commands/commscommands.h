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
 
 #ifndef COMMSCOMMANDS_H
#define COMMSCOMMANDS_H

#include <QObject>
#include <QSharedPointer>

#include <server/icommand.h>
#include <server/application/iapplication.h>
#include <server/application/iapplications.h>
#include <server/applicationregistry.h>
#include "headserverconnection.h"
#include "downloadableapplicationcache.h"

using namespace GBerry;
using namespace GBerry::Console::Server;

class ServerSideControlChannel;
class LocalApplicationsStorage;

namespace GBerryComms {

class CommsCommands : public QObject
{
    Q_OBJECT
public:
    CommsCommands(LocalApplicationsStorage* applicationStorage,
                  ApplicationRegistry* registry,
                  HeadServerConnection* headServerConnection,
                  DownloadableApplicationCache* _downloadableApplicationCache,
                  QObject *parent = 0);
    ~CommsCommands();

    ICommand* createLaunchApplicationCommand();
    ICommand* createExitApplicationCommand(int connectionId);
    ICommand* createQueryLocalApplicationsCommand();
    ICommand* createQueryDownloadableApplicationsCommand(ServerSideControlChannel* controlChannel);
    ICommand* createDownloadApplicationCommand(ServerSideControlChannel* controlChannel);
    ICommand* createHeadServerStatusCommand(ServerSideControlChannel* controlChannel);

signals:
    // TODO: should these go via command pattern, not as signals here
    void launchApplicationRequested(const QString applicationId);
    void exitApplicationRequested(const QString applicationId);

private:
    ApplicationRegistry* _applicationRegistry;
    HeadServerConnection* _headServerConnection;
    DownloadableApplicationCache* _downloadableApplicationCache;
    LocalApplicationsStorage* _applicationsStorage;
    QSharedPointer<IApplications> _iapps;

};

} // eon
#endif // COMMSCOMMANDS_H
