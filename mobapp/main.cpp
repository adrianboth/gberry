#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

//#include "appcontroller.h"

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>

#include "serverconnectionmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RealSystemServices systemServices;
    systemServices.registerItself();

    // this was original way
    //AppController appController;

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


    // TODO: console connection

    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextProperty("app", &appController);
    engine.rootContext()->setContextProperty("app", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    serverConnection.open();
    consoleConnection.open();

    return app.exec();
}
