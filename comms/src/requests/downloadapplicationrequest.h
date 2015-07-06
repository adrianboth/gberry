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
    explicit DownloadApplicationRequest(DownloadApplicationCommand* command, const QString& applicationFullId);
    virtual ~DownloadApplicationRequest();

    QString applicationFullId() const;

protected:
    virtual Invocation* processPrepare(RESTInvocationFactory* factory);
    virtual void processOkResponse(Invocation* invocation);
    virtual void processErrorResponse(Error err, Invocation* invocation);

private:
    DownloadApplicationCommand* _command;
    QString _applicationFullId;
};

} // eon

#endif // DOWNLOADAPPLICATIONREQUEST_H
