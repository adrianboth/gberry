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
 
 #ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QQmlApplicationEngine>

#include <applicationsettings.h>
#include <ui/displayprofile.h>

#include "client/clientsetup.h"
#include "client/4qml/assets.h"
#include "client/4qml/applicationmanager.h"
#include "client/4qml/gamemodel.h"
#include "client/4qml/downloadablegamesmodel.h"
#include "client/4qml/connection.h"
#include "client/4qml/downloadmodel.h"
#include "client/4qml/activeplayermodel.h"
#include "client/4qml/currentapplication.h"
#include "client/gamemodelcommunication.h"
#include "client/downloadablegamesmodelcommunication.h"
#include "client/downloadmodelcommunication.h"
#include "client/connectioncommunication.h"

using namespace GBerryApplication;

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s


class ConsoleApplication : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleApplication(QObject *parent = 0);
    ~ConsoleApplication();

    void run(QString mainQmlUrl);

    // introduced in *.h to get fresh compiler defines
    void setImportPaths(QString importPathList)
    {
        // for debug builds we run app from builds/ directory and want use
        // qml file tree from source folders
        qDebug() << "Setting debug build qml import path:" << importPathList;

        // currently paths can't contain spaces
        foreach(auto path, importPathList.split(":")) {
            _engine.addImportPath(path);
        }
    }

    Assets& assets() { return _assets; }
    CurrentApplication& currentApplication() { return _currentApplication; }

    void setApplicationCode(const QString& applicationCode);

    void setQmlContextProperty(const QString& key, const QVariant& value);


signals:

public slots:

private:
    QQmlApplicationEngine _engine;
    ClientSetup _setup;
    Assets _assets;
    DisplayProfile _displayProfile;
    ApplicationSettings _settings;

    GameModelCommunication _gameModelCommunication;
    GameModel _gameModel;
    DownloadableGamesModelCommunication _downloadableGamesModelCommunication;
    DownloadableGamesModel _downloadableGamesModel;
    DownloadModelCommunication _downloadModelCommunication;
    DownloadModel _downloadModel;
    ActivePlayerModel _activePlayerModel;
    CurrentApplication _currentApplication;

    ApplicationManager* _applicationManager;
    ConnectionCommunication _connectionCommunication;
    Connection _connection;

    QMap<QString, QVariant> _additionalQmlContextProperties;

};

#endif // CONSOLEAPPLICATION_H
