#include "usermodel.h"

#include <QSettings>
#include <QFile>
#include <QProcessEnvironment>

#define LOG_AREA "UserModel"
#include "log/log.h"

namespace {
    static const char* USERNAME_KEY = "username";
    static const char* PASSWORD_KEY = "password";
    static const char* GUEST_KEY = "guest";
    static const char* REMEMBER_PASSWORD_KEY = "remember_password";

    static const char* LAST_ACTIVE_USER = "last_active_user";
}

UserModel::UserModel(ApplicationStorage* storage, QObject* parent) :
    QObject(parent),
    _storage(storage),
    _activeUserIndex(-1),
    _ini(NULL)
{
    // reload from a file

    // create a file if not exists
    QString userIniFilePath(storage->storagePath() + "/usermodel.ini");

    // TODO: how to mock up this?
    QFile iniFile(userIniFilePath);
    _ini = new QSettings(userIniFilePath, QSettings::IniFormat);

    if (iniFile.exists()) {
        // read it
        DEBUG("Reading users from ini file");
        int index = 1;

        auto mkkey = [](const char* baseKey, int i) { QString key(baseKey);  key.append(QString::number(i)); return key; };

        while (true) {

            if (!_ini->contains(mkkey(USERNAME_KEY, index))) {
                // we expect ini file be valid i.e no gaps in users
                break;
            }
            UserInfo user; // initialized by defaults
            user.userName = _ini->value(mkkey(USERNAME_KEY, index)).toString();

            if (_ini->contains(mkkey(PASSWORD_KEY, index))) user.password = _ini->value(mkkey(PASSWORD_KEY, index)).toString();
            if (_ini->contains(mkkey(GUEST_KEY, index))) user.guest = _ini->value(mkkey(GUEST_KEY, index)).toBool();
            if (_ini->contains(mkkey(REMEMBER_PASSWORD_KEY, index))) user.rememberPassword = _ini->value(mkkey(REMEMBER_PASSWORD_KEY, index)).toBool();

            DEBUG("Read user: '" << user.userName << "'");
            _users.append(user);
            index++;
        }

        if (_ini->contains(LAST_ACTIVE_USER)) {
            // in file counting goes from 1 to X, but here we start from 0
            int readLastActiveUser = _ini->value(LAST_ACTIVE_USER).toInt() - 1;

            if (readLastActiveUser <= _users.length())
                _activeUserIndex = readLastActiveUser;
        }

        // as this is initial read, we are not notifying about the change

    } else {
        // just use defaults set by UserInfo constructor
        DEBUG("No saved data -> using defaults");
    }

}

UserModel::~UserModel()
{
    delete _ini;
}

bool UserModel::autoLoginEnabled() const
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("MOBAPP_AUTO_LOGIN")) {
        QString value = env.value("MOBAPP_AUTO_LOGIN").toLower().trimmed();

        if (value == "true" || value == "1")
            return true;
    }

    return false;
}

QString UserModel::currentUserName() const
{
    if (_activeUserIndex != -1) {
        return _users.at(_activeUserIndex).userName;
    } else {
        return "";
    }
}

QString UserModel::currentPassword() const
{
    if (_activeUserIndex != -1) {
        return _users.at(_activeUserIndex).password;
    } else {
        return "";
    }
}

bool UserModel::currentIsGuest() const
{
    if (_activeUserIndex != -1) {
        return _users.at(_activeUserIndex).guest;
    } else {
        return true;
    }
}

bool UserModel::currentIsRememberPassword() const
{
    if (_activeUserIndex != -1) {
        return _users.at(_activeUserIndex).rememberPassword;
    } else {
        return false;
    }
}

bool UserModel::selectCurrentUser(QString userName)
{
    for (int i = 0; i < _users.length(); i++) {
        if (_users.at(i).userName == userName) {
            _activeUserIndex = i;
            save();
            emit currentUserIsActiveChanged();
            emit currentUserNameChanged();
            return true;
        }
    }

    return false;
}

void UserModel::unselectCurrentUser() {
    _activeUserIndex = -1;
    save();
    emit currentUserIsActiveChanged();
    emit currentUserNameChanged();
}

QList<QString> UserModel::userNames() const
{
    QList<QString> names;
    foreach(auto user, _users) {
        names.append(user.userName);
    }

    return names;
}

QString UserModel::password(QString userName)
{
    if (updateCachedUser(userName)) {
        return _cachedUser.password;
    }
    // user not found
    return "";
}

bool UserModel::isGuest(QString userName)
{
    if (updateCachedUser(userName)) {
        return _cachedUser.guest;
    }
    // user not found
    return true;
}

bool UserModel::isRememberPassword(QString userName)
{
    if (updateCachedUser(userName)) {
        return _cachedUser.rememberPassword;
    }
    // user not found
    return false;
}

bool UserModel::updateCachedUser(QString &userName)
{
    if (_cachedUser.userName == userName) {
        // already in cache
        return true;
    }

    foreach(auto user, _users) {
        if (user.userName == userName) {
            _cachedUser = user;
            return true;
        }
    }

    return false;
}

void UserModel::setUser(UserInfo& userInfo)
{
    // TODO: we could take only those that has been changed
    for (int i = 0; i < _users.length(); i++) {
        UserInfo existingUser = _users.at(i);
        if (existingUser.userName == userInfo.userName) {
            _users.replace(i, userInfo);
            save();
            DEBUG("Saved existing a user: " << userInfo.userName)

            if (i == _activeUserIndex) {
                emit currentUserInfoChanged();
            }
            return;
        }
    }

    // this is a new user
    _users.append(userInfo);
    save();
    DEBUG("Created a new user: " << userInfo.userName)

}

void UserModel::save()
{
    // TODO: deleting / claering users
    auto mkkey = [](const char* baseKey, int i) { QString key(baseKey);  key.append(QString::number(i + 1)); return key; };

    for (int i = 0; i < _users.length(); i++) {
        _ini->setValue(mkkey(USERNAME_KEY, i), _users.at(i).userName);
        _ini->setValue(mkkey(PASSWORD_KEY, i), _users.at(i).password);
        _ini->setValue(mkkey(GUEST_KEY, i), _users.at(i).guest);
        _ini->setValue(mkkey(REMEMBER_PASSWORD_KEY, i), _users.at(i).rememberPassword);
    }

    _ini->setValue(LAST_ACTIVE_USER, _activeUserIndex + 1);

    _ini->sync();

    DEBUG("Saved user info");
}

void UserModel::setUser(QString userName, QString password, bool guest, bool rememberPassword)
{
    UserInfo userInfo(userName, password, guest, rememberPassword);
    setUser(userInfo);
}
