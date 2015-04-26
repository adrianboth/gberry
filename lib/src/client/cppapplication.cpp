#include "cppapplication.h"

#include <QQmlContext>

CppApplication::CppApplication(QQmlApplicationEngine* engine) :
    _engine(engine),
    _appStorage(engine),
    _userModel(NULL)
{
    _userModel = new UserModel(&_appStorage); // reads defaults when constructrd

    // TODO: should we delay reading user model data from disk later ...

    _engine->rootContext()->setContextProperty("UserModel", _userModel);
}

CppApplication::~CppApplication()
{
    delete _userModel;
}

ApplicationStorage& CppApplication::applicationStorage() {
    return _appStorage;
}
