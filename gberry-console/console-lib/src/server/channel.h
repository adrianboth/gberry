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
 
 #ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QMap>

#include "channelsouthpartner.h"
#include "icommand.h"
#include "lib_global.h"

/**
 * @brief The Channel class
 *
 * Message Flow
 * ------------
 *
 * Channel receives messages only specific for the channel:
 *
 *  - receiveMessage(..) is called when new message arrives
 *  - by default receiveMessage() tries to parse message as JSON
 *  - processJsonMessage(..) is called with successfully parsed JSON
 *  - by default processJsonMessage() delegates handling for channel parent via
 *    IChannelParent::processCommand(..)
 *  - ChannelManager (who is IChannelParent) checks if json msg has 'command'
 *     ember and if the member matches to any registered commands
 *      -> ICommand::process(..) is invoked.
 *
 * Subclasses of Channel class can override both receiveMessage(..) and
 * processCommand(..) to implement own handling.
 *
 */
class LIBSHARED_EXPORT Channel : public QObject
{
    Q_OBJECT

public:
    Channel(int channelId, ChannelSouthPartner* partner = 0, QObject* qParent = 0);
    virtual ~Channel();
    void attachSouthPartner(ChannelSouthPartner* partner);
    void detachSouthPartner();

    int channelId() const;

    virtual bool isOpen() const;
    virtual void open();
    // closing is more like pausing -> TODO: maybe better naming, TODO: playerchannels close, controlchannels are destroyed after closing
    virtual void close();

    // takes ownership of command
    void registerCommand(ICommand* cmd);

    virtual void sendJsonMessageToSouth(const QJsonObject& json);

public slots:
    // true if message was handled, used in subclasses to know if parent handled to message
    virtual bool receiveMessageFromSouth(const QByteArray& msg);

signals:
    void channelClosed();

protected:
    int _id;
    bool _isOpen;
    ChannelSouthPartner* _southPartner;
    QMap<QString, ICommand*> _commands;

    virtual bool processJsonMessage(const QJsonObject& json);
    void processCloseReceived();
    void sendMessageToSouth(const QByteArray& msg);

};

#endif // CHANNEL_H
