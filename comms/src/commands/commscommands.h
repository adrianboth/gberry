#ifndef COMMSCOMMANDS_H
#define COMMSCOMMANDS_H

#include <QObject>
#include <QSharedPointer>

#include <server/icommand.h>
#include <server/application/iapplication.h>
#include <server/application/iapplications.h>
#include <server/applicationregistry.h>

using namespace GBerry::Console::Server;


namespace GBerryComms {

class CommsCommands : public QObject
{
    Q_OBJECT
public:
    CommsCommands(QSharedPointer<IApplications> iapps,
                  ApplicationRegistry* registry,
                  QObject *parent = 0);
    ~CommsCommands();

    ICommand* createLaunchApplicationCommand();
    ICommand* createExitApplicationCommand(int connectionId);
    ICommand* createQueryLocalApplicationsCommand();

signals:
    void launchApplicationRequested(const QString applicationId);
    void exitApplicationRequested(const QString applicationId);

private:
    QSharedPointer<IApplications> _iapps;
    ApplicationRegistry* _applicationRegistry;

};

} // eon
#endif // COMMSCOMMANDS_H
