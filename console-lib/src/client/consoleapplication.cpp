#include "consoleapplication.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlContext>


ConsoleApplication::ConsoleApplication(QObject *parent) : QObject(parent)
{

}

ConsoleApplication::~ConsoleApplication()
{

}

void ConsoleApplication::run(QString mainQmlUrl)
{
    _setup.start();
    qDebug()<< "Import paths:" << _engine.importPathList();

    _engine.rootContext()->setContextProperty("playersManager", &(_setup.playersManager));
    _engine.rootContext()->setContextProperty("comms", &(_setup.controlChannel));

    _engine.load(QUrl(mainQmlUrl));

    QObject::connect(&_engine, &QQmlEngine::quit, QGuiApplication::instance(), QGuiApplication::quit);
}
