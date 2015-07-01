#ifndef DOWNLOABLEAPPLICATIONSREQUEST_H
#define DOWNLOABLEAPPLICATIONSREQUEST_H

#include "request.h"

namespace GBerry {

/**
 * DownloableApplicationsRequest makes REST query for applications that are
 * downloadable for a user.
 */
class DownloableApplicationsRequest : public Request
{
public:
    // TODO: user from somewhere (token)
    explicit DownloableApplicationsRequest();
    virtual ~DownloableApplicationsRequest();

    virtual void requestReady() override;
    virtual void requestFailed() override;
};

} // eon

#endif // DOWNLOABLEAPPLICATIONSREQUEST_H
