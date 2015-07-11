#include <QGuiApplication>
#include <client/applicationmain.h>
#include <client/consoleapplication.h>

using namespace GBerryApplication;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ApplicationMain main(&app);

    ConsoleApplication consoleApp;
    if (main.hasApplicationCode())
        consoleApp.setApplicationCode(main.applicationCode());

#ifdef GBERRY_DEBUG_QML_IMPORT_PATH
    consoleApp.setImportPaths(QString(xstr(GBERRY_DEBUG_QML_IMPORT_PATH)));
#endif

#ifdef GBERRY_ASSETS_FIXED_ROOT_PATH
    consoleApp.assets().setRootPath(QString(xstr(GBERRY_ASSETS_FIXED_ROOT_PATH)));
#endif

    consoleApp.run("qrc:/main.qml");

    return main.exec();
}
