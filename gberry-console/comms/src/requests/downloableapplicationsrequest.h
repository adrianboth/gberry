/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #ifndef DOWNLOABLEAPPLICATIONSREQUEST_H
#define DOWNLOABLEAPPLICATIONSREQUEST_H

#include <QObject>

#include "request.h"
using namespace GBerryLib;

#include "application.h"


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

    void setUserToken(int playerId, const QString& userToken);
    bool playerDefined() const;
    int playerId() const;

    QList<QSharedPointer<Application>> receivedApplications() const;

    void fillInErrorDetails(Result& res) override;

protected:
    virtual Invocation* processPrepare(InvocationFactory* factory) override;
    virtual void processOkResponse(Invocation* invocation) override;
    virtual void processErrorResponse(const Result& res, Invocation* invocation) override;

private:
    QueryDownloadableApplicationsCommand* _command;
    QList<QSharedPointer<Application>> _receivedApplications;
    int _playerId{-1};
    QString _userToken;
    QString _headServerHost;
};

} // eon

#endif // DOWNLOABLEAPPLICATIONSREQUEST_H
