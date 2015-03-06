#include <gtest/gtest.h>

#include "server/playersession.h"


TEST(PlayerSession, GuestPlayerSession)
{
    GuestPlayerSession g1("guest1", "abc123");
    EXPECT_TRUE(g1.playerId());

    GuestPlayerSession g2("guest2", "xyz567");
    EXPECT_TRUE(g1.playerId() != g2.playerId());
}
