#ifndef CPPAPPLICATION_H
#define CPPAPPLICATION_H

#include <QQmlApplicationEngine>

#include "applicationstorage.h"
#include "usermodel.h"

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

private:
    QQmlApplicationEngine* _engine;
    ApplicationStorage _appStorage;
    UserModel* _userModel;
};

#endif // CPPAPPLICATION_H
