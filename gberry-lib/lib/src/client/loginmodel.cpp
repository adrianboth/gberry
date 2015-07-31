#include "loginmodel.h"

#define LOG_AREA "LoginModel"
#include "log/log.h"

namespace GBerryClient {

class LoginModel::Private {
public:
    Private(UserModel* userModel_) :
        userModel(userModel_) {
        Q_ASSERT(userModel); // not null
    }

    UserModel* userModel;

    bool loggedIn{false}; // TODO: temp
};

LoginModel::LoginModel(UserModel* userModel) :
    _d(new Private(userModel))
{
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

    } else {
        _d->loggedIn = true;
        emit isLoggedInChanged();
        emit loginOk();
    }

    DEBUG("Login");
}

void GBerryClient::LoginModel::logout()
{
    _d->loggedIn = false;
    emit isLoggedInChanged();

    DEBUG("Logout");
}

void LoginModel::cancelLogin()
{
    // TODO
}

bool LoginModel::isLoggedIn() const
{
    return _d->loggedIn;
}

} // eon
