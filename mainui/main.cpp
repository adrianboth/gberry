#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

#include <client/clientsetup.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ClientSetup setup;
    setup.start();

    QQmlApplicationEngine engine;


    QDir::setSearchPaths("gb", QStringList("/home/tevuore/workspace/gberry/gberry-console/mainui/ui/gberry-lib"));

    engine.addImportPath("/home/tevuore/workspace/gberry/gberry-console/mainui/ui/gberry-lib");
    engine.addImportPath("/home/tevuore/workspace/gberry/gberry-console/mainui/testmodule");
    qDebug()<< "Import paths" << engine.importPathList();
    qDebug() << "baseUrl" << engine.rootContext()->baseUrl();
    engine.rootContext()->setContextProperty("playersManager", &(setup.playersManager));

    engine.rootContext()->setContextProperty("comms", &(setup.controlChannel));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    QObject::connect(&engine, &QQmlEngine::quit, &app, QGuiApplication::quit);

    return app.exec();
}
