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
 
 #include "consoleapplication.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlContext>
#include <ui/targetdisplay.h>


ConsoleApplication::ConsoleApplication(QObject *parent) :
    QObject(parent),
    _displayProfile(1920, 1080), // autoscale by default on

    _gameModelCommunication(_setup.controlChannel),
    _gameModel(&_gameModelCommunication),
    _downloadableGamesModelCommunication(_setup.controlChannel),
    _downloadableGamesModel(&_downloadableGamesModelCommunication, &_activePlayerModel),
    _downloadModelCommunication(_setup.controlChannel),
    _downloadModel(&_downloadModelCommunication, &_activePlayerModel),
    _connectionCommunication(_setup.controlChannel),
    _connection(&_connectionCommunication, _setup.controlChannel)
{

    if (!_settings.runningOnTargetDevice()) {
        TargetDisplay* targetDisplay = new TargetDisplay(1920, 1080, 110, 110, &_displayProfile);
        _displayProfile.useTargetDisplay(targetDisplay);

        // TODO: for dev time testing
        //_displayProfile.setMode(DisplayProfile::PixelMatch);
        //_displayProfile.setMode(DisplayProfile::DimensionMatch);
    }

    _applicationManager = new ApplicationManager(_setup.controlChannel);

    connect(&_downloadModel, &DownloadModel::downloadFinished,
            &_gameModel, &GameModel::onGameDownloaded);
}

ConsoleApplication::~ConsoleApplication()
{
}

void ConsoleApplication::run(QString mainQmlUrl)
{
    _setup.start();
    qDebug()<< "Import paths:" << _engine.importPathList();

    _engine.rootContext()->setContextProperty("playersManager", &(_setup.playersManager));
    // TODO: interface for communication status (OK, pinging, etc)
    //_engine.rootContext()->setContextProperty("comms", &(_setup.controlChannel));
    _engine.rootContext()->setContextProperty("Connection", &_connection);
    _engine.rootContext()->setContextProperty("Assets", &_assets);
    _engine.rootContext()->setContextProperty("DisplayProfile", &_displayProfile);
    _engine.rootContext()->setContextProperty("GameModel", &_gameModel);
    _engine.rootContext()->setContextProperty("DownloadableGamesModel", &_downloadableGamesModel);
    _engine.rootContext()->setContextProperty("DownloadModel", &_downloadModel);
    _engine.rootContext()->setContextProperty("ActivePlayerModel", &_activePlayerModel);
    _engine.rootContext()->setContextProperty("ApplicationSettings", &_settings);
    _engine.rootContext()->setContextProperty("ApplicationManager", _applicationManager);

    _engine.load(QUrl(mainQmlUrl));

    QObject::connect(&_engine, &QQmlEngine::quit, QGuiApplication::instance(), QGuiApplication::quit);
}

void ConsoleApplication::setApplicationCode(const QString& applicationCode)
{
    _setup.controlChannel->setApplicationCode(applicationCode);
}
