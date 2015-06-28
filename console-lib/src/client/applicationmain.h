#ifndef APPLICATIONMAIN_H
#define APPLICATIONMAIN_H

#include <QString>

class QGuiApplication;
class IEnvironmentVariables;

namespace GBerryApplication {

class ApplicationParameters;

class ApplicationMain
{
public:
    ApplicationMain(QGuiApplication* app);
    ~ApplicationMain();

    bool hasApplicationCode() const;
    QString applicationCode() const;

    // enters into event loop
    int exec();

private:
    const QGuiApplication* _app;
    IEnvironmentVariables* _env;
    ApplicationParameters* _params;

};

} // eon

#endif // APPLICATIONMAIN_H
