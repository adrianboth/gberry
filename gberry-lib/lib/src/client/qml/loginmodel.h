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
#ifndef LOGINMODEL_H
#define LOGINMODEL_H

#include <QObject>
#include <QScopedPointer>

#include "usermodel.h"
#include "invocationfactory.h"


namespace GBerryClient {

class LoginModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)
    //Q_PROPERTY(QString headServerHost READ headServerHost WRITE setHeadServerHost)

public:
    LoginModel(UserModel* userModel, IApplicationStorage* storage, InvocationFactory* invocationFactory);
    ~LoginModel();

    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();

    // cancel ongoing login
    Q_INVOKABLE void cancelLogin();

    QString userToken() const;

    // -- properties
    bool isLoggedIn() const;

    //QString headServerHost() const;
    //void setHeadServerHost(const QString& newHost);


signals:
    void isLoggedInChanged();

    void loginOk();
    void loginFailed(QString errorMessage);
    void logoutOk();
    void logoutFailed(QString errorMessage);

private:
    class Private;
    const QScopedPointer<Private> _d;
};

}

#endif // LOGINMODEL_H
