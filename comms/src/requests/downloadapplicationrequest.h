#ifndef DOWNLOADAPPLICATIONREQUEST_H
#define DOWNLOADAPPLICATIONREQUEST_H

#include <QObject>

#include "invocation.h"
#include "request.h"

namespace GBerry {

class DownloadApplicationCommand;

/**
 * DownloadApplicationRequest starts downloading a specified application from
 * head server.
 */
class DownloadApplicationRequest : public QObject, public Request
{
    Q_OBJECT
public:
    // TODO: user from somewhere (token)
    explicit DownloadApplicationRequest(
            DownloadApplicationCommand* command,
            const QString& applicationId,
            const QString& applicationVersion,
            const QString& destinationFilePath);
    virtual ~DownloadApplicationRequest();

    QString applicationFullId() const;
    QString destinationFilePath() const;

protected:
    virtual Invocation* processPrepare(InvocationFactory* factory);
    virtual void processOkResponse(Invocation* invocation);
    virtual void processErrorResponse(Error err, Invocation* invocation);

private:
    DownloadApplicationCommand* _command;
    QString _applicationId;
    QString _applicationVersion;
    QString _destinationFilePath;
};

} // eon

#endif // DOWNLOADAPPLICATIONREQUEST_H
