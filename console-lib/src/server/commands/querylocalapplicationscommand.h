#ifndef QUERYLOCALAPPLICATIONSCOMMAND_H
#define QUERYLOCALAPPLICATIONSCOMMAND_H

#include <QJsonObject>

#include "icommand.h"
#include "server/application/iapplications.h"

namespace GBerry {
    namespace Console {
        namespace Server {

class QueryLocalApplicationsCommandPrivate;

class QueryLocalApplicationsCommand : public ICommand
{
public:
    // TODO: how to create factory like
    QueryLocalApplicationsCommand(QSharedPointer<IApplications> apps);
    ~QueryLocalApplicationsCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

private:
    QueryLocalApplicationsCommandPrivate* _priv;

};

}}} // eon

#endif // QUERYLOCALAPPLICATIONSCOMMAND_H
