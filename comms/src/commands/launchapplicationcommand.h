#ifndef LAUNCHAPPLICATIONCOMMAND_H
#define LAUNCHAPPLICATIONCOMMAND_H

#include <QObject>
#include <QJsonObject>

#include <server/icommand.h>
#include <server/application/iapplications.h>
using namespace GBerry::Console::Server;

namespace GBerryComms {

class LaunchApplicationCommandPrivate;

class LaunchApplicationCommand : public ICommand
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
