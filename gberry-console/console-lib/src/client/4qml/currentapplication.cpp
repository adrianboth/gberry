#include "currentapplication.h"

#include <QJsonDocument>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>


#define LOG_AREA "CurrentApplication"
#include "log/log.h"

namespace GBerryApplication {

CurrentApplication::CurrentApplication()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("GBERRY_APPLICATION_ROOT")) {
        _rootPath = env.value("GBERRY_APPLICATION_ROOT");
    }
    else
    {
        _rootPath = QDir::current().absolutePath();
    }
    DEBUG("CurrentApplication root path:" << _rootPath);
    readConfig();
}

CurrentApplication::~CurrentApplication()
{

}

void CurrentApplication::setRootPath(const QString& rootPath)
{
    _rootPath = rootPath;
    readConfig();
}

QString CurrentApplication::applicationName() const
{
    if (_appcfg.contains("name")) {
        return _appcfg["name"].toString();
    }
    return "<undefined>";
}

QString CurrentApplication::applicationVersionString() const
{
    if (_appcfg.contains("version")) {
        return _appcfg["version"].toString();
    }
    return "<undefined>";
}

void CurrentApplication::readConfig() {
    _appcfg = QJsonObject(); // clear old
    QString cfgFilePath = findApplicationConfigFile(_rootPath);
    if (!cfgFilePath.isEmpty()) {
        QFile cfgFile(cfgFilePath);
        DEBUG("Reading" << cfgFilePath);

        if (!cfgFile.open(QIODevice::ReadOnly)) {
            ERROR("Failed to open" << cfgFile.fileName());
            return;
        }
        QJsonParseError error;
        QJsonDocument jdoc = QJsonDocument::fromJson(cfgFile.readAll(), &error);

        if (error.error != QJsonParseError::NoError) {
            ERROR("Failed to parse appcfg: " + error.errorString());
            return;
        }
        cfgFile.close();
        _appcfg = jdoc.object();

        // now we have something, not guaranteed that there is correct data
        //   -> validate
        if (!_appcfg.contains("name")) {
            ERROR("Couln't read 'name' from application config");
        }
        if (!_appcfg.contains("version")) {
            ERROR("Couln't read 'version' from application config");
        }

    } else {
       ERROR("Failed to read application configuration as configuration file was not found");
    }
}

// TODO: this is duplicate with ApplicatonConfigurationReaderWriter, how to have only once?
QString CurrentApplication::findApplicationConfigFile(const QString& applicationDirPath)
{
    QDir appDir(applicationDirPath);
    QStringList appcfgNameFilter;
    appcfgNameFilter << "*appcfg.json";
    appDir.setNameFilters(appcfgNameFilter);

    QStringList cfgFiles = appDir.entryList(QDir::Files);
    if (cfgFiles.length() == 0) {
        DEBUG("Read" << appDir.path() << "but didn't found *appcfg.json");
        return QString();
    }

    if (cfgFiles.length() > 1) {
        WARN("Application directory" << appDir.path() << "had several config files. Will select one:" << cfgFiles[0]);
        QString cfgFilePath = appDir.filePath(cfgFiles[0]);
        return cfgFilePath;
    }

    return appDir.filePath(cfgFiles[0]);
}

}
