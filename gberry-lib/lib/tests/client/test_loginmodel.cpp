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
#include <testutils/waiter.h>

#include <QTemporaryDir>
#include <QJsonObject>
#include <QJsonDocument>

#include "client/loginmodel.h"
#include "client/usermodel.h"
using namespace GBerryClient;

#include "utils/fileutils.h"

#include "testobjects/stubinvocationfactory.h"

class TempDirApplicationStorage : public IApplicationStorage {
public:

    virtual QString storagePath() const {
        return tempDir.path();
    }

    QTemporaryDir tempDir;
};

// no real http request
TEST(LoginModel, LoginOkLogoutOk)
{
    // fresh, not loading any ini files
    TempDirApplicationStorage storage;

    UserModel userModel(&storage); // no predefined users
    userModel.setUser("foouser",
                      "foo@mail",
                      "foopw",
                      false, //!guest
                      false);

    userModel.selectCurrentUser("foouser");

    StubInvocationFactory invocationFactory;

    LoginModel model(&userModel, &storage, &invocationFactory);

    ASSERT_FALSE(model.isLoggedIn());
    ASSERT_TRUE(model.userToken() == "");

    bool loginOkSignaled = false;
    QObject::connect(&model, &LoginModel::loginOk,
                     [&] () { loginOkSignaled = true; });
    model.login();

    WAIT_AND_ASSERT(invocationFactory.restInvs.size() == 1);

    StubRestInvocation* inv = invocationFactory.restInvs.takeFirst();
    ASSERT_TRUE(inv->executeCallCount == 1);
    ASSERT_TRUE(inv->paramInvocationPath == "/user/login") << inv->paramInvocationPath;
    QJsonObject json = inv->paramJsonDoc.object();
    ASSERT_TRUE(json["email"].toString() == "foo@mail");
    ASSERT_TRUE(json["password"].toString() == "foopw");

    // define response
    QJsonObject loginJsonResp;
    loginJsonResp["result"] = "ok";
    loginJsonResp["user_token"] = "1234";
    loginJsonResp["private_token"] = "abcd";
    inv->responseToReturn = QString(QJsonDocument(loginJsonResp).toJson());

    // trigger response and wait
    inv->emitFinishedOk();
    WAIT_AND_ASSERT(loginOkSignaled);
    ASSERT_TRUE(model.isLoggedIn());
    ASSERT_TRUE(model.userToken() == "1234");

    // cleanup
    delete inv;

// -- logout

    bool logoutOkSignaled = false;
    QObject::connect(&model, &LoginModel::logoutOk,
                     [&] () { logoutOkSignaled = true; });

    model.logout();

    WAIT_AND_ASSERT(invocationFactory.restInvs.size() == 1);

    inv = invocationFactory.restInvs.takeFirst();
    ASSERT_TRUE(inv->executeCallCount == 1);
    ASSERT_TRUE(inv->paramInvocationPath == "/user/logout") << inv->paramInvocationPath;
    ASSERT_TRUE(inv->parameters["private_token"] == "abcd") << inv->parameters["private_token"];

    // define response
    QJsonObject logoutJsonResp;
    logoutJsonResp["result"] = "ok";
    inv->responseToReturn = QString(QJsonDocument(logoutJsonResp).toJson());

    // trigger response and wait
    inv->emitFinishedOk();
    WAIT_AND_ASSERT(logoutOkSignaled);

    ASSERT_FALSE(model.isLoggedIn());
    ASSERT_TRUE(model.userToken() == "");

    // cleanup
    delete inv;
}


// test that tokens are read from a file
TEST(LoginModel, ReadLoginInfoFromIni) {
    TempDirApplicationStorage storage;
    QFile iniFile(GBerryLib::joinpath(storage.storagePath(), "loginmodel.ini"));
    ASSERT_TRUE(iniFile.open(QIODevice::WriteOnly));
    iniFile.write("version=1\n");
    iniFile.write("user_email=foo@mail\n");
    iniFile.write("user_token=1234\n");
    iniFile.write("private_token=abcd\n");
    iniFile.close();

    UserModel userModel(&storage); // no predefined users
    userModel.setUser("foouser",
                      "foo@mail",
                      "foopw",
                      false, //!guest
                      false);

    userModel.selectCurrentUser("foouser");

    StubInvocationFactory invocationFactory;

    LoginModel model(&userModel, &storage, &invocationFactory);

    // this the beef!
    ASSERT_TRUE(model.isLoggedIn());
    ASSERT_TRUE(model.userToken() == "1234");

// -- test logout resets ini data
    bool logoutOkSignaled = false;
    QObject::connect(&model, &LoginModel::logoutOk, [&] () { logoutOkSignaled = true; });
    model.logout();

    WAIT_AND_ASSERT(invocationFactory.restInvs.size() == 1);
    StubRestInvocation* inv = invocationFactory.restInvs.takeFirst();

    // define response
    QJsonObject logoutJsonResp;
    logoutJsonResp["result"] = "ok";
    inv->responseToReturn = QString(QJsonDocument(logoutJsonResp).toJson());

    // trigger response and wait
    inv->emitFinishedOk();
    WAIT_AND_ASSERT(logoutOkSignaled);

    // cleanup
    delete inv;

    // verify ini file
    iniFile.open(QIODevice::ReadOnly);
    QString data(iniFile.readAll());
    iniFile.close();
    ASSERT_TRUE(data.contains("user_email=\n")) << data;
    ASSERT_TRUE(data.contains("user_token=\n"));
    ASSERT_TRUE(data.contains("private_token=\n"));

// -- now change user and make login
    userModel.setUser("baruser",
                      "bar@mail",
                      "barpw",
                      false, //!guest
                      false);

    userModel.selectCurrentUser("baruser");

    bool loginOkSignaled = false;
    QObject::connect(&model, &LoginModel::loginOk,[&] () { loginOkSignaled = true; });
    model.login();

    WAIT_AND_ASSERT(invocationFactory.restInvs.size() == 1);
    inv = invocationFactory.restInvs.takeFirst();

    // define response
    QJsonObject loginJsonResp;
    loginJsonResp["result"] = "ok";
    loginJsonResp["user_token"] = "5678";
    loginJsonResp["private_token"] = "xyz";
    inv->responseToReturn = QString(QJsonDocument(loginJsonResp).toJson());

    // trigger response and wait
    inv->emitFinishedOk();
    WAIT_AND_ASSERT(loginOkSignaled);

    // cleanup
    delete inv;

    // verify file
    iniFile.open(QIODevice::ReadOnly);
    data = iniFile.readAll();
    iniFile.close();
    ASSERT_TRUE(data.contains("user_email=bar@mail\n")) << data;
    ASSERT_TRUE(data.contains("user_token=5678\n"));
    ASSERT_TRUE(data.contains("private_token=xyz\n"));

}
