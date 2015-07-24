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

#include "common/runtimeparameters.h"

#include <QProcessEnvironment>

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "testutils/mock_ienvironmentvariables.h"

using namespace GBerryConsoleCommon;

class TestRuntimeParameters : public RuntimeParameters
{
public:
    TestRuntimeParameters(IEnvironmentVariables& env) :
        RuntimeParameters(env) {
        addBooleanOption("foo", new QCommandLineOption("foo", "desc foo: only cmd", "foovalue"));

        addBooleanOption("foo2",  new QCommandLineOption("foo2", "desc foo2: option + env", "foo2value"));
        addEnvironmentVariable("foo2", "ENV_FOO2");

        addBooleanOption("foo3",  new QCommandLineOption("foo3", "desc foo3: option + env + default", "foo3value"));
        addEnvironmentVariable("foo3", "ENV_FOO3");
        addBuildTimeDefault("foo3", "default foo3");
    }
};


TEST(RuntimeParameters, IsSet)
{
    QMap<QString, QString> env;
    StubIEnvironmentVariables envStub(env);

    TestRuntimeParameters params(envStub);
    params.parse(QStringList() << "exename"); // no params

    EXPECT_FALSE(params.isSet("foo"));
    EXPECT_FALSE(params.isSet("foo2"));
    EXPECT_TRUE(params.isSet("foo3")); // from default
    EXPECT_TRUE(params.value("foo3") == "default foo3");

// -- command line parameters
    QStringList opts;
    opts << "exename" << "--foo=1" << "--foo2=2" << "--foo3=3";
    params.parse(opts);

    EXPECT_TRUE(params.isSet("foo"));
    EXPECT_TRUE(params.isSet("foo2"));
    EXPECT_TRUE(params.isSet("foo3"));
    EXPECT_TRUE(params.value("foo") == "1");
    EXPECT_TRUE(params.value("foo2") == "2");
    EXPECT_TRUE(params.value("foo3") == "3");

// -- from env
    env["ENV_FOO2"] = "E2";
    env["ENV_FOO3"] = "E3";

    params.parse(QStringList() << "exename" << "--foo2=2");

    EXPECT_TRUE(params.value("foo2") == "2"); // cmdline overrides env
    EXPECT_TRUE(params.value("foo3") == "E3");

}
