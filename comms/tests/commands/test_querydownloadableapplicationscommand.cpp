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

#include "commands/querydownloadableapplicationscommand.h"

using namespace GBerry;

class TestCommandResponse : public ICommandResponse
{
public:
    void set(QJsonObject json_) override { json = json_; }
    QJsonObject json;
};


TEST(QueryDownloadableApplicationsCommand, OK)
{
    /*
    DownloadableApplicationCache applicationCache; // so simple that no mock need ...
    HeadServerConnection headServerConnection; // TODO: some interface for this
    ServerSideControlChannel controlChannel; // TODO: some interface for message sending

    QueryDownloadableApplicationsCommand cmd();
    */
}

