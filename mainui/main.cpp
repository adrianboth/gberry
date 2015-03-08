#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <client/clientsetup.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ClientSetup setup;
    setup.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("playersManager", &(setup.playersManager));

    engine.rootContext()->setContextProperty("comms", &(setup.controlChannel));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
