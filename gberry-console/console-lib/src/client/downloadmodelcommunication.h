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
 
#ifndef DOWNLOADMODELCOMMUNICATION_H
#define DOWNLOADMODELCOMMUNICATION_H

// TODO: could communication classes be moved to common subfolder ...

#include <QObject>
#include <QJsonObject>

#include "client/4qml/downloadmodel.h"

class ClientSideControlChannel;

namespace GBerryApplication {

class DownloadModelCommunication : public IDownloadModelCommunication
{
    Q_OBJECT
public:
    explicit DownloadModelCommunication(ClientSideControlChannel* controlChannel,
                                        QObject *parent = 0);
    virtual ~DownloadModelCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};

} // eon

#endif // DOWNLOADMODELCOMMUNICATION_H
