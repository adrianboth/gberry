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
