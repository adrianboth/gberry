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
 
#include <gtest/gtest.h>

#include "server/playersession.h"


TEST(PlayerSession, GuestPlayerSession)
{
    GuestPlayerSession g1("guest1", "abc123");
    EXPECT_TRUE(g1.playerId() < -1);
    EXPECT_TRUE(g1.isValid());
    EXPECT_TRUE(g1.isGuest());
    EXPECT_TRUE(g1.sessionToken() == "abc123");
    EXPECT_TRUE(g1.userToken().isEmpty());

    GuestPlayerSession g2("guest2", "xyz567");
    EXPECT_TRUE(g1.playerId() != g2.playerId());
}

TEST(PlayerSession, SignedInPlayerSession)
{
    SignedInPlayerSession g1("player1", "abc123", "xyz567");
    EXPECT_TRUE(g1.playerId() > 0);
    EXPECT_TRUE(g1.isValid());
    EXPECT_FALSE(g1.isGuest());
    EXPECT_TRUE(g1.sessionToken() == "abc123");
    EXPECT_TRUE(g1.userToken() == "xyz567");
}

TEST(PlayerSession, CopyConstructorAndAssigment)
{
    PlayerSession s1(1, "name1", "token1", "token2");
    PlayerSession s2(s1);

    EXPECT_TRUE(s1.playerId() == s2.playerId());
    EXPECT_TRUE(s1.isValid() == s2.isValid());
    EXPECT_TRUE(s1.isGuest() == s2.isGuest());
    EXPECT_TRUE(s1.sessionToken() == s2.sessionToken());
    EXPECT_TRUE(s1.userToken() == s2.userToken());

    PlayerSession s3;
    s3 = s1;
    EXPECT_TRUE(s1.playerId() == s3.playerId());
    EXPECT_TRUE(s1.isValid() == s3.isValid());
    EXPECT_TRUE(s1.isGuest() == s3.isGuest());
    EXPECT_TRUE(s1.sessionToken() == s3.sessionToken());
    EXPECT_TRUE(s1.userToken() == s3.userToken());
}
