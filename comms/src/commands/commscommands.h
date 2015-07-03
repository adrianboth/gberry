#ifndef COMMSCOMMANDS_H
#define COMMSCOMMANDS_H

#include <QObject>
#include <QSharedPointer>

#include <server/icommand.h>
#include <server/application/iapplication.h>
#include <server/application/iapplications.h>
#include <server/applicationregistry.h>
#include "headserverconnection.h"

using namespace GBerry;
using namespace GBerry::Console::Server;

class ServerSideControlChannel;

namespace GBerryComms {

class CommsCommands : public QObject
{
    Q_OBJECT
public:
    CommsCommands(QSharedPointer<IApplications> iapps,
                  ApplicationRegistry* registry,
                  HeadServerConnection* headServerConnection,
                  QObject *parent = 0);
    ~CommsCommands();

    ICommand* createLaunchApplicationCommand();
    ICommand* createExitApplicationCommand(int connectionId);
    ICommand* createQueryLocalApplicationsCommand();
    ICommand* createQueryDownloadableApplicationsCommand(ServerSideControlChannel* controlChannel);

signals:
    void launchApplicationRequested(const QString applicationId);
    void exitApplicationRequested(const QString applicationId);

private:
    QSharedPointer<IApplications> _iapps;
    ApplicationRegistry* _applicationRegistry;
    HeadServerConnection* _headServerConnection;

};

} // eon
#endif // COMMSCOMMANDS_H
