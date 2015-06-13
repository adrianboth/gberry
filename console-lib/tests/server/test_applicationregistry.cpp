#include <testutils/qtgtest.h>

#include "server/applicationregistry.h"

using namespace GBerry::Console::Server;

TEST(ApplicationRegistry, BasicUseCase)
{
    ApplicationRegistry reg;
    EXPECT_TRUE(reg.matchCodeToApplication("abc") == "");
    EXPECT_TRUE(reg.matchCodeToApplication("") == "");

    QString code = reg.createIdentificationCode("fooapp");
    // still valid match required
    EXPECT_TRUE(reg.matchCodeToApplication("abc") == "");
    EXPECT_TRUE(reg.matchCodeToApplication(code) == "fooapp");
    EXPECT_TRUE(reg.matchCodeToApplication(code) == ""); // onetime code
}
