#include <gtest/gtest.h>

#include "systemservices.h"

#include "testutils/signalrecorder.h"


TEST(SystemServices, defaultInstance)
{
    SystemServices* services = SystemServices::instance();
    EXPECT_TRUE(services != NULL);

    // dummy impls do nothing
    services->singleshotTimer(100, services, "foobarMember");
}
