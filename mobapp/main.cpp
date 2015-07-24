/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>
#include <QProcessEnvironment>

#include <invocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>

#include "serverconnectionmodel.h"

#include <client/qmlapplication.h>
#include <client/cppapplication.h>

#define LOG_AREA "MobApp"
#include "log/log.h"
#include <log/logcontrol.h>
#include <log/stdoutlogmsghandler.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    StdoutLogMsgHandler handler(Log::TRACE);
    LogControl logControl;
    logControl.registerMsgHandler(&handler);
    Log::singleton().use(&logControl);

    DEBUG("Starting");

    RealSystemServices systemServices;
    systemServices.registerItself();

    InvocationFactoryImpl factory;
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
    mobile::QmlApplication mobapp;

    QQmlPropertyMap screenProps;

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

    QQmlApplicationEngine engine;

    CppApplication cppApp(&engine);

    engine.addImportPath("qrc:/ui/gberry-lib");

    engine.rootContext()->setContextProperty("app", &model);
    engine.rootContext()->setContextProperty("mobapp", &mobapp);
    engine.rootContext()->setContextProperty("screen", &screenProps);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    // TODO: temp disable
    //serverConnection.open();
    //consoleConnection.open();

    return app.exec();
}
