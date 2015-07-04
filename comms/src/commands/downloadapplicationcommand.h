#ifndef DOWNLOADAPPLICATIONCOMMAND_H
#define DOWNLOADAPPLICATIONCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>

#include <server/icommand.h>

class ServerSideControlChannel;

namespace GBerry {

class HeadServerConnection;
class DownloadApplicationCommandPrivate;
class DownloadApplicationRequest;

/**
 * @brief The QueryDownloadableApplicationsCommand class
 *
 * There will be one instance of this class for each ControlChannel.
 */
class DownloadApplicationCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    // TODO: how to create factory like
    explicit DownloadApplicationCommand(
            HeadServerConnection* headServerConnection,
            ServerSideControlChannel* controlChannel);
    virtual ~DownloadApplicationCommand();

    // ICommand
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;


    // --

    // callback from Request
    void processRequestOkResponse(DownloadApplicationRequest* request);
    void processRequestErrorResponse(DownloadApplicationRequest* request);

private:
    const QScopedPointer<DownloadApplicationCommandPrivate> _d;

};

} // eon

#endif // DOWNLOADAPPLICATIONCOMMAND_H
