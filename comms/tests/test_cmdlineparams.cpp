#include <gtest/gtest.h>

#include "cmdlineparams.h"

#include <QProcessEnvironment>

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "testutils/mock_ienvironmentvariables.h"
#include "testutils/qtgtest.h"
#include "testutils/util_enablelog.h"

namespace {
    static TestUtils::StdoutLoggingEnabler& logging = TestUtils::enabledStdoutLogging();
}

using namespace Comms;

TEST(CmdLineParams, ValidOption)
{
    QMap<QString, QString> env;
    StubIEnvironmentVariables envStub(env);

    CmdLineParams params(envStub);
    params.parse(QStringList() << "exename"); // no params

    EXPECT_FALSE(params.isSet(CmdLineParams::DISABLE_MAINUI));

    EXPECT_TRUE(params.value(CmdLineParams::ROOT_PATH).length() > 0); // build time path

    // --
    params.parse(QStringList() << "exename" << "--disable-main-ui");
    EXPECT_TRUE(params.isSet(CmdLineParams::DISABLE_MAINUI));

    // --
    params.parse(QStringList() << "exename");
    env["GBERRY_ROOT_PATH"] = "testpath";
    //EXPECT_TRUE(env.contains("GBERRY_ROOT_PATH"));
    //EXPECT_TRUE(envStub.contains("GBERRY_ROOT_PATH"));

    EXPECT_TRUE(params.isSet(CmdLineParams::ROOT_PATH));
    EXPECT_TRUE(params.value(CmdLineParams::ROOT_PATH) == "testpath") << params.value(CmdLineParams::ROOT_PATH);

    params.parse(QStringList() << "exename" << "--root-path=foobar");
    EXPECT_TRUE(params.isSet(CmdLineParams::ROOT_PATH));
    EXPECT_TRUE(params.value(CmdLineParams::ROOT_PATH) == "foobar");

}
