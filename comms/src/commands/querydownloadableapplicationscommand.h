#ifndef QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
#define QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>

#include <server/icommand.h>

class ServerSideControlChannel;

namespace GBerryComms {
class DownloadableApplicationCache;
}
using namespace GBerryComms;

namespace GBerry {

class HeadServerConnection;
class QueryDownloadableApplicationsCommandPrivate;
class DownloadableApplicationsRequest;


/**
 * @brief The QueryDownloadableApplicationsCommand class
 *
 * There will be one instance of this class for each ControlChannel.
 */
class QueryDownloadableApplicationsCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    // TODO: how to create factory like
    explicit QueryDownloadableApplicationsCommand(
            HeadServerConnection* headServerConnection,
            ServerSideControlChannel* controlChannel,
            DownloadableApplicationCache* applicationCache);
    virtual ~QueryDownloadableApplicationsCommand();

    // ICommand
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;


    // --

    // callback from Request
    void processRequestOkResponse(DownloadableApplicationsRequest* request);
    void processRequestErrorResponse(DownloadableApplicationsRequest* request);

private:
    const QScopedPointer<QueryDownloadableApplicationsCommandPrivate> _d;

};

} // eon

#endif // QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
