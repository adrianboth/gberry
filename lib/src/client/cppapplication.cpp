#include "cppapplication.h"

#include <QQmlContext>
#include <QDebug>

#include "ui/targetdisplay.h"

CppApplication::CppApplication(QQmlApplicationEngine* engine) :
    _engine(engine),
    _appStorage(engine),
    _userModel(NULL),
    _displayProfile(480, 800, DisplayProfile::PixelMatch, 1.0) // Nexus S, portrait
{
    _userModel = new UserModel(&_appStorage); // reads defaults when constructed

    if (!_settings.runningOnTargetDevice()) {
        qDebug() << "[CppApplication] Setting target display for development";
        TargetDisplay* targetDisplay = new TargetDisplay(480, 800, 223, 223); // default nexus s
        _displayProfile.useTargetDisplay(targetDisplay);
        //_displayProfile.setMode(DisplayProfile::DimensionMatch);
    }

    // TODO: should we delay reading user model data from disk later ...

    _engine->rootContext()->setContextProperty("UserModel", _userModel);
    _engine->rootContext()->setContextProperty("DisplayProfile", &_displayProfile);
    _engine->rootContext()->setContextProperty("ApplicationSettings", &_settings);
}

CppApplication::~CppApplication()
{
    delete _userModel;
}

ApplicationStorage& CppApplication::applicationStorage() {
    return _appStorage;
}
