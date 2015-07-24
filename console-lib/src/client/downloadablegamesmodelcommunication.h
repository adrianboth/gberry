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
 
 #ifndef DOWNLOADABLEGAMESMODELCOMMUNICATION_H
#define DOWNLOADABLEGAMESMODELCOMMUNICATION_H

#include <QObject>

#include "client/4qml/downloadablegamesmodel.h"

class ClientSideControlChannel;

namespace GBerry {

class DownloadableGamesModelCommunication : public IDownloadableGamesModelCommunication
{
    Q_OBJECT
public:
    explicit DownloadableGamesModelCommunication(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    virtual ~DownloadableGamesModelCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};

} // eon

#endif // DOWNLOADABLEGAMESMODELCOMMUNICATION_H
