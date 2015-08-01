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
 
 #ifndef CPPAPPLICATION_H
#define CPPAPPLICATION_H

#include <QQmlApplicationEngine>

#include "applicationstorage.h"
#include "client/qml/usermodel.h"
#include "client/qml/loginmodel.h"
#include "ui/displayprofile.h"
#include "applicationsettings.h"
#include "invocationfactoryimpl.h"

namespace GBerryClient {

/**
 * @brief Internal application model
 *
 * This class is application model for C++ side only but it contains also
 * initialization of QmlApplication.
 */
class CppApplication
{
public:
    CppApplication(QQmlApplicationEngine* engine);
    ~CppApplication();

    ApplicationStorage& applicationStorage();
    UserModel& userModel();
    LoginModel& loginModel();

private:
    QQmlApplicationEngine* _engine;
    InvocationFactoryImpl _invocationFactory;
    ApplicationStorage _appStorage;
    UserModel* _userModel{nullptr};
    LoginModel* _loginModel{nullptr};
    DisplayProfile _displayProfile;
    ApplicationSettings _settings;
};

} // eon

#endif // CPPAPPLICATION_H
