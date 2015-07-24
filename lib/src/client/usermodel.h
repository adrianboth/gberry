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
 
 #ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QSettings>

#include "applicationstorage.h"

class UserInfo {

public:
    UserInfo() : userName(""), password(""), guest(true), rememberPassword(false) {}

    UserInfo(QString& userName_, QString& password_, bool guest_, bool rememberPassword_) :
        userName(userName_), password(password_), guest(guest_), rememberPassword(rememberPassword_) {}

    UserInfo(const UserInfo& other) :
        userName(other.userName), password(other.password), guest(other.guest), rememberPassword(other.rememberPassword) {}

    UserInfo& operator=(const UserInfo& other) {
        userName = other.userName;
        password = other.password;
        guest = other.guest;
        rememberPassword = other.rememberPassword;
        return *this;
    }

    QString userName;
    QString password;
    bool guest;
    bool rememberPassword;

};

class UserModel : public QObject
{
    Q_OBJECT

    // intefacing towards QML happens through properties
    Q_PROPERTY(bool currentUserIsActive READ currentUserIsActive NOTIFY currentUserIsActiveChanged)
    Q_PROPERTY(QString currentUserName READ currentUserName NOTIFY currentUserNameChanged)
    Q_PROPERTY(QString currentPassword READ currentPassword)
    Q_PROPERTY(QString currentIsGuest READ currentIsGuest)
    Q_PROPERTY(QString currentIsRememberPassword READ currentIsRememberPassword)

    // dev time util for making automatic login
    Q_PROPERTY(bool autoLoginEnabled READ autoLoginEnabled)

public:
    UserModel(ApplicationStorage* storage, QObject* parent = NULL);
    ~UserModel();

    // -- properties
    bool currentUserIsActive() { return _activeUserIndex != -1; }
    QString currentUserName() const;
    QString currentPassword() const ;
    bool currentIsGuest() const;
    bool currentIsRememberPassword() const;

    bool autoLoginEnabled() const;

    // -- functions

    Q_INVOKABLE bool selectCurrentUser(QString userName);
    Q_INVOKABLE void unselectCurrentUser();

    Q_INVOKABLE QList<QString> userNames() const;
    Q_INVOKABLE QString password(QString userName);
    Q_INVOKABLE bool isGuest(QString userName);
    Q_INVOKABLE bool isRememberPassword(QString userName);

    void setUser(UserInfo& userInfo);
    Q_INVOKABLE void setUser(QString userName, QString password, bool guest, bool rememberPassword);

signals:
    void currentUserInfoChanged();
    void currentUserNameChanged();
    void currentUserIsActiveChanged();

private:
    void save();
    bool updateCachedUser(QString& userName);

    ApplicationStorage* _storage;
    int _activeUserIndex;
    QList<UserInfo> _users;
    QSettings* _ini;
    UserInfo _cachedUser;

};

#endif // USERMODEL_H
