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
 
 #include "cppapplication.h"

#include <QQmlContext>
#include <QDebug>

#include "ui/targetdisplay.h"
#include "applicationstorage.h"

namespace GBerryClient {

CppApplication::CppApplication(QQmlApplicationEngine* engine) :
    _engine(engine),
    _appStorage(engine),
    _displayProfile(480, 800, DisplayProfile::PixelMatch, 1.0) // Nexus S, portrait
{
    _userModel = new UserModel(&_appStorage); // reads defaults when constructed
    _loginModel = new LoginModel(_userModel, &_appStorage, &_invocationFactory);

    if (!_settings.runningOnTargetDevice()) {
        qDebug() << "[CppApplication] Setting target display for development";
        TargetDisplay* targetDisplay = new TargetDisplay(480, 800, 223, 223); // default nexus s
        _displayProfile.useTargetDisplay(targetDisplay);
        //_displayProfile.setMode(DisplayProfile::DimensionMatch);
    }

    // TODO: should we delay reading user model data from disk later ...

    _engine->rootContext()->setContextProperty("UserModel", _userModel);
    _engine->rootContext()->setContextProperty("LoginModel", _loginModel);
    _engine->rootContext()->setContextProperty("DisplayProfile", &_displayProfile);
    _engine->rootContext()->setContextProperty("ApplicationSettings", &_settings);
}

CppApplication::~CppApplication()
{
    delete _loginModel;
    delete _userModel;
}

ApplicationStorage& CppApplication::applicationStorage() {
    return _appStorage;
}

UserModel &CppApplication::userModel()
{
    return *_userModel;
}

LoginModel &CppApplication::loginModel()
{
    return *_loginModel;
}

} // eon
