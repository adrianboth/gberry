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
    Q_PROPERTY(QString currentUserName READ currentUserName NOTIFY currentUserNameChanged)
    Q_PROPERTY(QString currentPassword READ currentPassword)
    Q_PROPERTY(QString currentIsGuest READ currentIsGuest)
    Q_PROPERTY(QString currentIsRememberPassword READ currentIsRememberPassword)

    // dev time util for making automatic login
    Q_PROPERTY(bool autoLoginEnabled READ autoLoginEnabled)

public:
    UserModel(ApplicationStorage* storage, QObject* parent = NULL);
    ~UserModel();

    QString currentUserName() const { return _currentInfo.userName; }
    QString currentPassword() const { return _currentInfo.password; }
    bool currentIsGuest() const { return _currentInfo.guest; }
    bool currentIsRememberPassword() const { return _currentInfo.rememberPassword; }

    bool autoLoginEnabled();

    void setCurrent(UserInfo& userInfo);
    Q_INVOKABLE void setCurrent(QString userName, QString password, bool guest, bool rememberPassword);

signals:
    void currentUserInfoChanged();
    void currentUserNameChanged();

private:
    ApplicationStorage* _storage;
    UserInfo _currentInfo;
    QSettings* _ini;

};

#endif // USERMODEL_H
