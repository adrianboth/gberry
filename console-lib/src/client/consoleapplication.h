#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <applicationsettings.h>
#include <ui/displayprofile.h>

#include "client/clientsetup.h"
#include "client/4qml/assets.h"
#include "client/4qml/applicationmanager.h"
#include "client/4qml/gamemodel.h"
#include "client/4qml/downloadablegamesmodel.h"
#include "client/4qml/connection.h"
#include "client/gamemodelcommunication.h"
#include "client/downloadablegamesmodelcommunication.h"

using namespace GBerry;

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

    void setApplicationCode(const QString& applicationCode);

signals:

public slots:

private:
    QQmlApplicationEngine _engine;
    ClientSetup _setup;
    Assets _assets;
    DisplayProfile _displayProfile;
    GameModelCommunication _gameModelCommunication;
    GameModel _gameModel;
    DownloadableGamesModelCommunication _downloadableGamesModelCommunication;
    DownloadableGamesModel _downloadableGamesModel;
    ApplicationSettings _settings;
    ApplicationManager* _applicationManager;
    Connection _connection;

};

#endif // CONSOLEAPPLICATION_H
