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
 
 #ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include <QByteArray>
#include <QJsonObject>

#include "common/playermeta.h"
#include "server/application/iapplications.h"
using namespace GBerry::Console::Server;

namespace GBerry {
namespace Console {

class MessageFactory
{
public:

    static QByteArray createPingCommand();
    static QByteArray createPingReply();
    static QByteArray createPingCommand(const QString& applicationIdCode);
    static QByteArray createPingReply(const QString& applicationIdCode);
    static QJsonObject createPingCommandJson();
    static QJsonObject createPingReplyJson();

    static QByteArray createOpenChannelCommand(PlayerMeta PlayerMeta);
    static QByteArray createOpenChannelReply();
    static QByteArray createCloseChannelCommand();

    static QByteArray createPlayerMessage(QJsonObject playerData);
    static QByteArray createPlayerMessage(QString str);

    static QJsonObject createQueryLocalApplicationsReply(QSharedPointer<IApplications> apps);

private:
    MessageFactory();
    ~MessageFactory();
};

}} // eon

#endif // MESSAGEFACTORY_H
