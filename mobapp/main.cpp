#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>
#include <QProcessEnvironment>

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>

#include "serverconnectionmodel.h"

#include "client/application.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RealSystemServices systemServices;
    systemServices.registerItself();

    RESTInvocationFactoryImpl factory;
    factory.setProperty("url_prefix", "http://localhost:9000/gberryrest/v1");

    ServerConnectionImpl serverConnection(&factory);
    ServerConnectionModel model;

    QObject::connect(&serverConnection, &ServerConnectionImpl::pingOK,
                     &model,            &ServerConnectionModel::serverPingOK);
    QObject::connect(&serverConnection, &ServerConnectionImpl::pingFailure,
                     &model,            &ServerConnectionModel::serverPingFailure);
    QObject::connect(&serverConnection, &ServerConnectionImpl::connected,
                     &model,            &ServerConnectionModel::serverConnected);
    QObject::connect(&serverConnection, &ServerConnectionImpl::disconnected,
                     &model,            &ServerConnectionModel::serverDisconnected);
/*
    RESTInvocationFactoryImpl factory2;
    factory2.setProperty("url_prefix", "http://localhost:8050/console/v1");

    ServerConnectionImpl consoleConnection(&factory2);

    QObject::connect(&consoleConnection, &ServerConnectionImpl::pingOK,
                     &model,            &ServerConnectionModel::consolePingOK);
    QObject::connect(&consoleConnection, &ServerConnectionImpl::pingFailure,
                     &model,            &ServerConnectionModel::consolePingFailure);
    QObject::connect(&consoleConnection, &ServerConnectionImpl::connected,
                     &model,            &ServerConnectionModel::consoleConnected);
    QObject::connect(&consoleConnection, &ServerConnectionImpl::disconnected,
                     &model,            &ServerConnectionModel::consoleDisconnected);
*/
    mobile::Application mobapp;

    //mobile::ConsoleDevice console("localhost");

    //mobapp.loginGuest("GuestFoo");
    //mobapp.openConsoleConnection(console);

    QQmlPropertyMap screenProps;
    screenProps.insert("name", QVariant(QString("John Smith")));


    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    if (env.contains("PREF_WINDOW_WIDTH")) {
        screenProps.insert("preferredWindowWidth", QVariant(env.value("PREF_WINDOW_WIDTH").toInt()));
    } else {
        screenProps.insert("preferredWindowWidth", QVariant(320));
    }

    if (env.contains("PREF_WINDOW_HEIGHT")) {
        screenProps.insert("preferredWindowHeight", QVariant(env.value("PREF_WINDOW_HEIGHT").toInt()));
    } else {
        screenProps.insert("preferredWindowHeight", QVariant(400));
    }

    // TODO: env for login -> faster setup

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/ui/gberry-lib");

    engine.rootContext()->setContextProperty("app", &model);
    engine.rootContext()->setContextProperty("mobapp", &mobapp);
    engine.rootContext()->setContextProperty("screen", &screenProps);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    qDebug() << "QQmlEngine::offlineStoragePath()" << engine.offlineStoragePath();
    // TODO: temp disable
    //serverConnection.open();
    //consoleConnection.open();

    return app.exec();
}
