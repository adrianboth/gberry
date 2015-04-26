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
}

UserModel::UserModel(ApplicationStorage* storage, QObject* parent) :
    QObject(parent),
    _storage(storage),
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
        if (_ini->contains(USERNAME_KEY)) {
            _currentInfo.userName = _ini->value(USERNAME_KEY).toString();
            DEBUG("Read: '" << _ini->value(USERNAME_KEY).toString() << "'");
        }
        if (_ini->contains(PASSWORD_KEY)) _currentInfo.password = _ini->value(PASSWORD_KEY).toString();
        if (_ini->contains(GUEST_KEY)) _currentInfo.guest = _ini->value(GUEST_KEY).toBool();
        if (_ini->contains(REMEMBER_PASSWORD_KEY)) _currentInfo.rememberPassword = _ini->value(REMEMBER_PASSWORD_KEY).toBool();

        DEBUG("Read user: '" << _currentInfo.userName << "'");

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

bool UserModel::autoLoginEnabled()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("MOBAPP_AUTO_LOGIN")) {
        QString value = env.value("MOBAPP_AUTO_LOGIN").toLower().trimmed();

        if (value == "true" || value == "1")
            return true;
    }

    return false;
}

void UserModel::setCurrent(UserInfo& userInfo)
{
    // TODO: we could take only those that has been changed
    _currentInfo = userInfo;
    _ini->setValue(USERNAME_KEY, _currentInfo.userName);
    _ini->setValue(PASSWORD_KEY, _currentInfo.password);
    _ini->setValue(GUEST_KEY, _currentInfo.guest);
    _ini->setValue(REMEMBER_PASSWORD_KEY, _currentInfo.rememberPassword);
    _ini->sync();

    DEBUG("Saved user info");
    emit currentUserInfoChanged();
    emit currentUserNameChanged();
}

void UserModel::setCurrent(QString userName, QString password, bool guest, bool rememberPassword)
{
    UserInfo userInfo(userName, password, guest, rememberPassword);
    setCurrent(userInfo);
}
