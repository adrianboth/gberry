#include <QGuiApplication>

#include <client/consoleapplication.h>


int main(int argc, char *argv[])
{
    // TODO: This all should be wrapped into common class
    QGuiApplication app(argc, argv);
    ConsoleApplication consoleApp;

#ifdef GBERRY_DEBUG_QML_IMPORT_PATH
    consoleApp.setImportPaths(QString(xstr(GBERRY_DEBUG_QML_IMPORT_PATH)));
#endif
    consoleApp.run("qrc:/main.qml");

    return app.exec();
}
