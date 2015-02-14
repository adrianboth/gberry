#include <QCoreApplication>
#include <QDebug>
#include <QUrl>

#include "gberrymaincontroller.h"
#include "gberryconsoleserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "HELLO!";

    GBerryConsoleServer server;
    GBerryMainController controller;
    controller.start();
    //controller.startRequest(QUrl("http://localhost:9000/gberryrest/v1/ping/"));

    return a.exec();
}
