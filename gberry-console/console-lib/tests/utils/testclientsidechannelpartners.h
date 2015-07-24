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
 
 #ifndef TESTCLIENTSIDECHANNELPARTNERS_H
#define TESTCLIENTSIDECHANNELPARTNERS_H

#include "client/clientsidechannelpartners.h"

class TestClientSidePlayerChannelPartner : public ClientSidePlayerChannelPartner
{
public:
    explicit TestClientSidePlayerChannelPartner(int channelId) :
        ClientSidePlayerChannelPartner(channelId),
        playerMessageCount(0),
        playerExitCalledCount(0) {}
    virtual ~TestClientSidePlayerChannelPartner() {}

    int playerMessageCount;
    QByteArray lastMessage;

    int playerExitCalledCount;
    virtual void playerMessage(const QByteArray& msg) override {
        playerMessageCount++;
        lastMessage = msg;
    }

    virtual void playerExit() override {
        playerExitCalledCount++;
    }
};

class TestClientSideChannelPartner : public ClientSideChannelPartner
{
public:
    explicit TestClientSideChannelPartner(int channelId) :
        ClientSideChannelPartner(channelId),
        messageCount(0),
        closeChannelCalled(0) {}
    virtual ~TestClientSideChannelPartner() {}

    int messageCount;
    QByteArray lastMessage;

    int closeChannelCalled;

    virtual void sendMessage(const QByteArray& msg) override {
        messageCount++;
        lastMessage = msg;
    }
    virtual void closeChannel() override {
        closeChannelCalled++;
    }
};
#endif // TESTCLIENTSIDECHANNELPARTNERS_H

