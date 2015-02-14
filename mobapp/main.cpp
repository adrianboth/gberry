#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    AppController appController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("app", &appController);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
