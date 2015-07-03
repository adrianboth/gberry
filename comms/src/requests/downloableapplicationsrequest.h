#ifndef DOWNLOABLEAPPLICATIONSREQUEST_H
#define DOWNLOABLEAPPLICATIONSREQUEST_H

#include <QObject>

#include "request.h"


namespace GBerry {

class QueryDownloadableApplicationsCommand;

/**
 * DownloableApplicationsRequest makes REST query for applications that are
 * downloadable for a user.
 */
class DownloadableApplicationsRequest : public QObject, public Request
{
    Q_OBJECT
public:
    // TODO: user from somewhere (token)
    explicit DownloadableApplicationsRequest(QueryDownloadableApplicationsCommand* command);
    virtual ~DownloadableApplicationsRequest();

protected:
    virtual void processPrepare(RESTInvocation* invocation);
    virtual void processOkResponse(RESTInvocation* invocation);
    virtual void processErrorResponse(RESTInvocation* invocation);

private:
    QueryDownloadableApplicationsCommand* _command;
};

} // eon

#endif // DOWNLOABLEAPPLICATIONSREQUEST_H
