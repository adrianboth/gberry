#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include "gberryconsoleserver.h"

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>
#include <server/commtcpserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RealSystemServices systemServices;
    systemServices.registerItself();

    GBerryConsoleServer server;

    RESTInvocationFactoryImpl factory;
    factory.setProperty("url_prefix", "http://localhost:9000/gberryrest/v1");

    ServerConnectionImpl serverConnection(&factory);

    auto pingOKSlot = [&]() { qDebug("pingOK"); };

    QObject::connect(&serverConnection, &ServerConnectionImpl::pingOK, pingOKSlot);
    /*
    QObject::connect(&serverConnection, &ServerConnectionImpl::pingFailure,
                     &model,            &ServerConnectionModel::serverPingFailure);
    QObject::connect(&serverConnection, &ServerConnectionImpl::connected,
                     &model,            &ServerConnectionModel::serverConnected);
    QObject::connect(&serverConnection, &ServerConnectionImpl::disconnected,
                     &model,            &ServerConnectionModel::serverDisconnected);
    */
    serverConnection.open();

    CommTcpServer comms(7000);
    comms.open();

    auto func = [&comms] (int connectionId, const QByteArray& msg) {
        QByteArray a(QString("echo" + QString(msg)).toLatin1());
        comms.write(connectionId, a);
    };
    QObject::connect(&comms, &CommTcpServer::received, func);

    return a.exec();
}
