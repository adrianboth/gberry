#ifndef LOGINMODEL_H
#define LOGINMODEL_H

#include <QObject>
#include <QScopedPointer>

#include "usermodel.h"

namespace GBerryClient {

class LoginModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)
public:
    LoginModel(UserModel* userModel);
    ~LoginModel();

    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();

    // cancel ongoing login
    Q_INVOKABLE void cancelLogin();

    bool isLoggedIn() const;


signals:
    void isLoggedInChanged();

    void loginOk();
    void loginFailed(QString errorMessage);

private:
    class Private;
    const QScopedPointer<Private> _d;
};

}

#endif // LOGINMODEL_H
