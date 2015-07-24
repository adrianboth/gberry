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
 
 
#include <testutils/qtgtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include <testutils/waiter.h>

#include "server/connectionmanager.h"
#include "server/channelfactory.h"
#include "common/messagefactory.h"

#include "utils/testchannel.h"
#include "utils/testchannelfactory.h"
#include "utils/testplayerchannelnorthpartner.h"
#include "utils/util_messageparser.h"

#define LOG_AREA "test_connectionmanager"
#include "log/log.h"

using namespace GBerry::Console;
using namespace GBerry::Console::Server;

TEST(ConnectionManager, ValidateConnection)
{
    //ConnectionManager manager;
}
