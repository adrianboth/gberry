#include "consoleapplication.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlContext>
#include <ui/targetdisplay.h>


ConsoleApplication::ConsoleApplication(QObject *parent) :
    QObject(parent),
    _displayProfile(1920, 1080) // autoscale by default on
{
    if (!_settings.runningOnTargetDevice()) {
        TargetDisplay* targetDisplay = new TargetDisplay(1920, 1080, 110, 110, &_displayProfile);
        _displayProfile.useTargetDisplay(targetDisplay);

        // TODO: for dev time testing
        //_displayProfile.setMode(DisplayProfile::PixelMatch);
        //_displayProfile.setMode(DisplayProfile::DimensionMatch);
    }

    _applicationManager = new ApplicationManager(_setup.controlChannel);
}

ConsoleApplication::~ConsoleApplication()
{

}

void ConsoleApplication::run(QString mainQmlUrl)
{
    _setup.start();
    qDebug()<< "Import paths:" << _engine.importPathList();

    _engine.rootContext()->setContextProperty("playersManager", &(_setup.playersManager));
    //_engine.rootContext()->setContextProperty("comms", &(_setup.controlChannel));
    _engine.rootContext()->setContextProperty("Assets", &_assets);
    _engine.rootContext()->setContextProperty("DisplayProfile", &_displayProfile);
    _engine.rootContext()->setContextProperty("GameModel", &_gameModel);
    _engine.rootContext()->setContextProperty("ApplicationSettings", &_settings);
    _engine.rootContext()->setContextProperty("ApplicationManager", _applicationManager);

    _engine.load(QUrl(mainQmlUrl));

    QObject::connect(&_engine, &QQmlEngine::quit, QGuiApplication::instance(), QGuiApplication::quit);
}
