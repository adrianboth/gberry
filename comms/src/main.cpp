#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include "gberryconsoleserver.h"

#include <restinvocationfactoryimpl.h>
#include <serverconnectionimpl.h>
#include <realsystemservices.h>
#include <server/commtcpserver.h>

#include <server/serversetup.h>
#include <server/playersessionmanager.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RealSystemServices systemServices;
    systemServices.registerItself();

    ServerSetup setup;
    setup.start();

    return a.exec();
}
