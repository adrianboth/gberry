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
 
 #ifndef TESTCHANNEL_H
#define TESTCHANNEL_H

#include "server/channel.h"


class TestChannel : public Channel
{
    Q_OBJECT

public:
    TestChannel(int channelId);
    ~TestChannel();

    void triggerOutgoingMessage(QByteArray msg);
    int received;
    QString lastReceivedMsg;
    QByteArray lastSentMsg;

    // Channel
    virtual void reopen() {}

public slots:
    // Channel
    bool receiveMessageFromSouth(const QByteArray msg);

};

#endif // TESTCHANNEL_H
