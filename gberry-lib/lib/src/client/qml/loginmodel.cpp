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
#include "loginmodel.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QSettings>

#define LOG_AREA "LoginModel"
#include "log/log.h"

#include "client/iapplicationstorage.h"
#include "invocationfactoryimpl.h"
#include "restinvocation.h"
#include "resultmessageformatter.h"

namespace {
    static const char* FILE_VERSION_KEY = "version";
    static const char* FILE_VERSION_VALUE = "1"; // update this when significant changes that you need to handle

    static const char* USEREMAIL_KEY = "user_email"; // email is login id
    static const char* USERTOKEN_KEY = "user_token";
    static const char* PRIVATETOKEN_KEY = "private_token";
}

namespace GBerryClient {

class LoginModel::Private {
public:
    Private(LoginModel* q_, UserModel* userModel_, IApplicationStorage* storage_, InvocationFactory* invocationFactory_) :
        q(q_), userModel(userModel_), storage(storage_), invocationFactory(invocationFactory_) {
        Q_ASSERT(userModel); // not null
    }

    LoginModel* q;
    UserModel* userModel;
    IApplicationStorage* storage;
    InvocationFactory* invocationFactory;
    QSettings* ini{nullptr};

    bool loggedIn{false};
    QString currentUserEmail;
    QString currentUserToken;
    QString currentPrivateToken;

    void loginInvocationFinishedOk(RESTInvocation* inv) {

        QJsonParseError error;
        QByteArray data(inv->responseString().toLatin1());
        QJsonObject json = QJsonDocument::fromJson(data, &error).object();

        if (error.error != QJsonParseError::NoError) {
            // login failed
            ERROR("Failed to parse ok login response:" << error.errorString());
            loginInvocationFinishedError(inv, "Failed to parse login respose"); // TODO: i18n
            return;
        }

        QString result = json["result"].toString();
        if (result != "ok") {
            ERROR("Login result not ok:" << result);
            loginInvocationFinishedError(inv, "Login result not ok"); // TODO: i18n + details
            return;
        }

        currentUserToken = json["user_token"].toString();
        currentPrivateToken = json["private_token"].toString();
        currentUserEmail = userModel->currentEmail();
        saveData();

        inv->deleteLater();
        loggedIn = true;
        emit q->isLoggedInChanged();
        emit q->loginOk();
    }

    void loginInvocationFinishedError(RESTInvocation* inv, const QString& errorString = QString()) {
        loggedIn = false;
        emit q->isLoggedInChanged();
        if (errorString.isEmpty()) {
            emit q->loginFailed(ResultMessageFormatter(inv->result()).createEndUserMessage());
        } else {
            // TODO: this chain of calls doesn't produce localized message
            emit q->loginFailed(errorString);
        }

        inv->deleteLater();
    }

    void logoutInvocationFinishedOk(RESTInvocation* inv) {

        QJsonParseError error;
        QByteArray data(inv->responseString().toLatin1());
        QJsonObject json = QJsonDocument::fromJson(data, &error).object();

        if (error.error != QJsonParseError::NoError) {
            // login failed
            ERROR("Failed to parse ok logout response:" << error.errorString());
            logoutInvocationFinishedError(inv, "Failed to parse logout response"); // TODO: i18n + details
            return;
        }

        QString result = json["result"].toString();
        if (result != "ok") {
            ERROR("Logout result not ok:" << result);
            logoutInvocationFinishedError(inv, "Logout result not ok"); // TODO: i18n + details
            return;
        }

        inv->deleteLater();
        loggedIn = false;
        emit q->isLoggedInChanged();
        emit q->logoutOk();
    }

    void logoutInvocationFinishedError(RESTInvocation* inv, const QString& errorString = QString()) {
        loggedIn = false;
        emit q->isLoggedInChanged();
        if (errorString.isEmpty()) {
            emit q->loginFailed(ResultMessageFormatter(inv->result()).createEndUserMessage());
        } else {
            // TODO: this chain of calls doesn't produce localized message
            emit q->loginFailed(errorString);
        }
        inv->deleteLater();
    }

    void saveData() {
        if (ini) {
            ini->setValue(USEREMAIL_KEY, currentUserEmail);
            ini->setValue(USERTOKEN_KEY, currentUserToken);
            ini->setValue(PRIVATETOKEN_KEY, currentPrivateToken);
            ini->sync();
        }
    }
};

LoginModel::LoginModel(UserModel* userModel, IApplicationStorage* storage, InvocationFactory* invocationFactory) :
    _d(new Private(this, userModel, storage, invocationFactory))
{
    connect(userModel, &UserModel::currentUserNameChanged,
            [&] () { this->logout();} );

    // create a file if not exists
    QString loginIniFilePath(storage->storagePath() + "/loginmodel.ini");

    // TODO: how to mock up this?
    QFile iniFile(loginIniFilePath);
    _d->ini = new QSettings(loginIniFilePath, QSettings::IniFormat);

    if (iniFile.exists()) {
        DEBUG("Reading saved login from storage");

        if (_d->ini->contains(FILE_VERSION_KEY)) {
            QString version = _d->ini->value(FILE_VERSION_KEY).toString();
            DEBUG("Users ini file version:" << version);
        } else {
            DEBUG("No version information in Users ini file");
            // set version and it will be saved when users data is saved next time
            _d->ini->setValue(FILE_VERSION_KEY, FILE_VERSION_VALUE);
        }

        // read all only if we have all define (otherwise corrupted data)
        if (_d->ini->contains(USEREMAIL_KEY) && _d->ini->contains(USERTOKEN_KEY) && _d->ini->contains(PRIVATETOKEN_KEY) ) {
            QString userEmail = _d->ini->value(USEREMAIL_KEY).toString();
            // use tokens only if the current user is same
            if (_d->userModel->currentEmail() == userEmail) {
                DEBUG("User emails matched - reading saved tokens");
                _d->currentUserEmail = userEmail;
                _d->currentUserToken = _d->ini->value(USERTOKEN_KEY).toString();
                _d->currentPrivateToken = _d->ini->value(PRIVATETOKEN_KEY).toString();
                _d->loggedIn = true;
            } else {
                DEBUG("User emails did not match: Discarding saved tokens");
            }
        } else {
            ERROR("Invalid data from" << loginIniFilePath << ". All keys didn't have values.")
        }

    } else {
        // no file, no saved login
    }
}

LoginModel::~LoginModel()
{
}

void GBerryClient::LoginModel::login()
{
    // TODO: temporary testing
    if (_d->userModel->currentUserName() == "fail") {
        _d->loggedIn = false;
        emit isLoggedInChanged();
        emit loginFailed("test failure");

    } else if (_d->userModel->currentUserName() == "login") {
        _d->loggedIn = true;
        emit isLoggedInChanged();
        emit loginOk();

    } else {
        if (_d->loggedIn)
            logout();

        RESTInvocation* inv = _d->invocationFactory->newRESTInvocation();

        connect(inv, &RESTInvocation::finishedOK,
                [=] (Invocation* i) { Q_UNUSED(i); _d->loginInvocationFinishedOk(inv); } );
        connect(inv, &RESTInvocation::finishedError,
                [=] (Invocation* i) { Q_UNUSED(i); _d->loginInvocationFinishedError(inv); });

        // curl -v -H "Content-Type: application/json" -X POST --data "{\"email\": \"$1\", \"password\": \"$2\"}" http://${GBERRYHOST}/gberryrest/v1/user/login

        QJsonObject json;
        json["email"] = _d->userModel->currentEmail();
        json["password"] = _d->userModel->currentPassword();
        inv->definePostOperation("/user/login", QJsonDocument(json));

        inv->execute();
    }

    DEBUG("Login");
}

void GBerryClient::LoginModel::logout()
{
    DEBUG("Logout");
    if (_d->loggedIn) {
        QString privateToken = _d->currentPrivateToken;
        _d->currentPrivateToken = "";
        _d->currentUserToken = "";
        _d->currentUserEmail = "";
        _d->saveData();

        RESTInvocation* inv = _d->invocationFactory->newRESTInvocation();

        connect(inv, &RESTInvocation::finishedOK,
                [=] (Invocation* i) { Q_UNUSED(i); _d->logoutInvocationFinishedOk(inv); } );
        connect(inv, &RESTInvocation::finishedError,
                [=] (Invocation* i) { Q_UNUSED(i); _d->logoutInvocationFinishedError(inv); });

        // curl -v -X GET  http://localhost:8001/gberryrest/v1/user/logout?private_token=$1

        inv->defineGetOperation("/user/logout");
        inv->defineParameter("private_token", privateToken);

        inv->execute();
    }

}

void LoginModel::cancelLogin()
{
    // TODO
}

QString LoginModel::userToken() const
{
    return _d->currentUserToken; // if not valid then empty
}

bool LoginModel::isLoggedIn() const
{
    return _d->loggedIn;
}

QString LoginModel::headServerHost() const
{
    // TODO: this in fact return whole url -> but I guess it does hurt at this point as no one is using (just for debugging)
    QVariant urlPrefix = _d->invocationFactory->property("url_prefix");
    if (urlPrefix.isValid()) {
        return urlPrefix.toString();
    } else {
        return "";
    }
}

void LoginModel::setHeadServerHost(const QString &newHost)
{
    _d->invocationFactory->setProperty("url_prefix", "http://" + newHost + "/gberryrest/v1");
}

} // eon
