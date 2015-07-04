#ifndef DOWNLOADAPPLICATIONREQUEST_H
#define DOWNLOADAPPLICATIONREQUEST_H

#include <QObject>

#include "request.h"

namespace GBerry {

class DownloadableApplicationCommand;

/**
 * DownloadApplicationRequest starts downloading a specified application from
 * head server.
 */
class DownloadApplicationRequest : public QObject, public Request
{
    Q_OBJECT
public:
    // TODO: user from somewhere (token)
    explicit DownloadApplicationRequest(DownloadApplicationCommand* command);
    virtual ~DownloadApplicationRequest();

protected:
    virtual void processPrepare(RESTInvocation* invocation);
    virtual void processOkResponse(RESTInvocation* invocation);
    virtual void processErrorResponse(RESTInvocation* invocation);

private:
    DownloadApplicationCommand* _command;
};

} // eon

#endif // DOWNLOADAPPLICATIONREQUEST_H
