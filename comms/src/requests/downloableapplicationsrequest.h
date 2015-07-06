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
    virtual Invocation* processPrepare(InvocationFactory* factory);
    virtual void processOkResponse(Invocation* invocation);
    virtual void processErrorResponse(Request::Error error, Invocation* invocation);

private:
    QueryDownloadableApplicationsCommand* _command;
};

} // eon

#endif // DOWNLOABLEAPPLICATIONSREQUEST_H
