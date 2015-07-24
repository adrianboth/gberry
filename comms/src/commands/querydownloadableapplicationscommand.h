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
 
 #ifndef QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
#define QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>

#include <server/icommand.h>
#include <result.h>

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
    void processRequestErrorResponse(
            DownloadableApplicationsRequest* request,
            const GBerryLib::Result& result);

private:
    const QScopedPointer<QueryDownloadableApplicationsCommandPrivate> _d;

};

} // eon

#endif // QUERYDOWNLOADABLEAPPLICATIONSCOMMAND_H
