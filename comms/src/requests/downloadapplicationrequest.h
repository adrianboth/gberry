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
    virtual Invocation* processPrepare(InvocationFactory* factory) override;
    virtual void processOkResponse(Invocation* invocation) override;
    virtual void processErrorResponse(const Result& result, Invocation* invocation) override;

private:
    DownloadApplicationCommand* _command;
    QString _applicationId;
    QString _applicationVersion;
    QString _destinationFilePath;
};

} // eon

#endif // DOWNLOADAPPLICATIONREQUEST_H
