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
 
 #ifndef ICLIENTSIDECHANNELPARTNER_H
#define ICLIENTSIDECHANNELPARTNER_H

class QByteArray;

class ClientSideChannelPartner
{
public:
    explicit ClientSideChannelPartner(int channelId) : _channelId(channelId) {}
    virtual ~ClientSideChannelPartner() {}
    virtual void sendMessage(const QByteArray& msg) = 0;
    virtual void closeChannel() = 0;

protected:
    int _channelId;

};

class ClientSidePlayerChannelPartner
{
public:
    explicit ClientSidePlayerChannelPartner(int channelId) : _channelId(channelId) {}
    virtual ~ClientSidePlayerChannelPartner() {}

    virtual void playerMessage(const QByteArray& msg) = 0;
    virtual void playerExit() = 0;

protected:
    int _channelId;
};

#endif // ICLIENTSIDECHANNELPARTNER_H

