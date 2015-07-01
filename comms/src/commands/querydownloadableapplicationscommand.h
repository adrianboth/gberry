#ifndef QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
#define QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>

#include <server/icommand.h>

namespace GBerry {

class QueryDownloadableApplicationsCommandPrivate;

class QueryDownloadableApplicationsCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    // TODO: how to create factory like
    explicit QueryDownloadableApplicationsCommand();
    virtual ~QueryDownloadableApplicationsCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

private:
    const QScopedPointer<QueryDownloadableApplicationsCommandPrivate> _d;

};

} // eon

#endif // QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
