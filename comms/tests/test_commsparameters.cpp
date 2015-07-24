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

#include "commsparameters.h"

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

TEST(CommsParameters, ValidOption)
{
    QMap<QString, QString> env;
    StubIEnvironmentVariables envStub(env);

    CommsParameters params(envStub);
    params.parse(QStringList() << "exename"); // no params

    EXPECT_FALSE(params.isSet(CommsParameters::DISABLE_MAINUI));

    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH).length() > 0); // build time path

    // --
    params.parse(QStringList() << "exename" << "--disable-main-ui");
    EXPECT_TRUE(params.isSet(CommsParameters::DISABLE_MAINUI));

    // --
    params.parse(QStringList() << "exename");
    env["GBERRY_ROOT_PATH"] = "testpath";
    //EXPECT_TRUE(env.contains("GBERRY_ROOT_PATH"));
    //EXPECT_TRUE(envStub.contains("GBERRY_ROOT_PATH"));

    EXPECT_TRUE(params.isSet(CommsParameters::ROOT_PATH));
    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH) == "testpath") << params.value(CommsParameters::ROOT_PATH);

    params.parse(QStringList() << "exename" << "--root-path=foobar");
    EXPECT_TRUE(params.isSet(CommsParameters::ROOT_PATH));
    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH) == "foobar");

}

TEST(CommsParameters, IsSet)
{
    QMap<QString, QString> env;
    StubIEnvironmentVariables envStub(env);

    CommsParameters params(envStub);
    params.parse(QStringList() << "exename"); // no params

    EXPECT_FALSE(params.isSet(CommsParameters::DISABLE_MAINUI));

    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH).length() > 0); // build time path

    // --
    params.parse(QStringList() << "exename" << "--disable-main-ui");
    EXPECT_TRUE(params.isSet(CommsParameters::DISABLE_MAINUI));

    // --
    params.parse(QStringList() << "exename");
    env["GBERRY_ROOT_PATH"] = "testpath";
    //EXPECT_TRUE(env.contains("GBERRY_ROOT_PATH"));
    //EXPECT_TRUE(envStub.contains("GBERRY_ROOT_PATH"));

    EXPECT_TRUE(params.isSet(CommsParameters::ROOT_PATH));
    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH) == "testpath") << params.value(CommsParameters::ROOT_PATH);

    params.parse(QStringList() << "exename" << "--root-path=foobar");
    EXPECT_TRUE(params.isSet(CommsParameters::ROOT_PATH));
    EXPECT_TRUE(params.value(CommsParameters::ROOT_PATH) == "foobar");

}
