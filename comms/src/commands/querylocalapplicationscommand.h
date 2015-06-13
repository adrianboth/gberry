#ifndef QUERYLOCALAPPLICATIONSCOMMAND_H
#define QUERYLOCALAPPLICATIONSCOMMAND_H

#include <QJsonObject>

#include <server/icommand.h>
#include <server/application/iapplications.h>

using namespace GBerry::Console::Server;

namespace GBerryComms {

class QueryLocalApplicationsCommandPrivate;

class QueryLocalApplicationsCommand : public ICommand
{
    Q_OBJECT

public:
    // TODO: how to create factory like
    QueryLocalApplicationsCommand(QSharedPointer<IApplications> apps);
    ~QueryLocalApplicationsCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

private:
    QueryLocalApplicationsCommandPrivate* _priv;

};

} // eon

#endif // QUERYLOCALAPPLICATIONSCOMMAND_H
