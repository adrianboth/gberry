#include "applicationconfigreaderwriter.h"

#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QDir>

#include "json/jsonvalidator.h"
#include "json/jsondefinition.h"
#include "json/jsondefinitionbuilder.h"

#define LOG_AREA "ApplicationConfigReaderWriter"
#include "log/log.h"

namespace
{
    static const char* JSON_APPCFG_ID = "id";
    static const char* JSON_APPCFG_NAME = "name";
    static const char* JSON_APPCFG_VERSION = "version";
    static const char* JSON_APPCFG_DESCRIPTION = "description";
    static const char* JSON_APPCFG_CATALOGIMAGE = "catalog_image";
    static const char* JSON_APPCFG_APPLICATIONEXE = "application_exe";
    static const char* JSON_APPCFG_SYSTEMAPP = "system_app";
    static const char* JSON_APPCFG_DEVELOPERCOMMENTS = "developer_comments";

    static const char* JSON_STATE = "state";
}

QSharedPointer<JsonDefinition> createApplicationConfigDefinition()
{
    JsonDefinitionBuilder builder;

    builder.hasStringMember(JSON_APPCFG_ID);
    builder.hasStringMember(JSON_APPCFG_DESCRIPTION);
    builder.hasStringMember(JSON_APPCFG_NAME);
    builder.hasOptionalStringMember(JSON_APPCFG_CATALOGIMAGE);
    builder.hasOptionalStringMember(JSON_APPCFG_APPLICATIONEXE);
    builder.hasOptionalStringMember(JSON_APPCFG_DEVELOPERCOMMENTS);

    builder.hasOptionalBooleanMember(JSON_APPCFG_SYSTEMAPP);

    return builder.definition();
}

QSharedPointer<JsonDefinition> createApplicationStateDefinition()
{
    JsonDefinitionBuilder builder;
    builder.hasStringMember(JSON_STATE);
    return builder.definition();
}

QSharedPointer<JsonDefinition> JsonDef(createApplicationConfigDefinition());
QSharedPointer<JsonDefinition> StateJsonDef(createApplicationStateDefinition());


ApplicationConfigReaderWriter::ApplicationConfigReaderWriter(const QString& applicationDir) :
    _applicationDir(applicationDir)
{
}

ApplicationConfigReaderWriter::~ApplicationConfigReaderWriter()
{
}

QSharedPointer<Application> ApplicationConfigReaderWriter::readApplicationConfig(const QString &configFilePath)
{
    QFile file(configFilePath);
    QDir tmp(configFilePath);
    tmp.cdUp();
    QDir applicationDir(tmp);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        WARN("Failed to open " << configFilePath << ": " << file.errorString());
        return QSharedPointer<Application>(nullptr);
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc(QJsonDocument::fromJson(data, &err));
    //DEBUG("JSON:" << QString(data));
    //DEBUG("ERROR:" << err.errorString());

    QJsonObject json(doc.object());
    //DEBUG("ISARRAY:" << doc.isArray());
    //DEBUG("ISOBJECT:" << doc.isObject());
    JsonValidator jsonValidator(JsonDef);
    if (!jsonValidator.validate(json)) {
        // had errors
        DEBUG("Discarded appcfg " << configFilePath << " as it had errors: " << jsonValidator.errors());
        return QSharedPointer<Application>(nullptr);
    }

    QSharedPointer<ApplicationMeta> meta(new ApplicationMeta);
    QSharedPointer<Application> app(new Application(meta));

    auto readStr = [&] (const char* key) { return json[key].toString(); };
    auto readBool = [&] (const char* key) { return json[key].toBool(); };

    meta->setApplicationId(readStr(JSON_APPCFG_ID));
    meta->setName(readStr(JSON_APPCFG_NAME));
    meta->setVersion(readStr(JSON_APPCFG_VERSION));
    meta->setDescription(readStr(JSON_APPCFG_DESCRIPTION));
    meta->setApplicationDirPath(applicationDir.absolutePath());

    if (json.contains(JSON_APPCFG_APPLICATIONEXE)) {
        meta->setApplicationExecutablePath(applicationDir.absoluteFilePath(readStr(JSON_APPCFG_APPLICATIONEXE)));
    } else {
        meta->setApplicationExecutablePath(applicationDir.absoluteFilePath(meta->applicationId()));
    }

    if (json.contains(JSON_APPCFG_CATALOGIMAGE)) {
        meta->setCatalogImageFilePath(applicationDir.absoluteFilePath(readStr(JSON_APPCFG_CATALOGIMAGE)));
    }

    if (json.contains(JSON_APPCFG_SYSTEMAPP)) {
        meta->setIsSystemApp(readBool(JSON_APPCFG_SYSTEMAPP));
    }

    // TODO: id in cfg in nicely mathcing to id() we are using
    // TODO: optional with default value
    //   * builder using def
    // TODO: optional checking type

    // TODO: rest setters

    return app;
}


QSharedPointer<Application> ApplicationConfigReaderWriter::readApplication()
{
    QStringList appcfgNameFilter;
    appcfgNameFilter << "*appcfg.json";

    QDir d(_applicationDir);
    d.setNameFilters(appcfgNameFilter);
    QStringList cfgFiles = d.entryList(QDir::Files);
    if (cfgFiles.length() == 0) {
        DEBUG("Read" << d.path() << "but didn't found *appcfg.json");
        return QSharedPointer<Application>(nullptr);

    } else if (cfgFiles.length() > 1) {
        WARN("Application directory" << d.path() << "had several config files. Will select one:" << cfgFiles[0]);
    }

    QString cfgFilePath = d.filePath(cfgFiles[0]);
    QSharedPointer<Application> app(readApplicationConfig(cfgFilePath));

    QString stateFilePath = d.filePath("state.json");
    readApplicationState(stateFilePath, *app.data());

    return app;
}

void ApplicationConfigReaderWriter::writeApplication(const Application &application)
{
    // TODO:
}

void ApplicationConfigReaderWriter::readApplicationState(const QString &stateFilePath, Application &application)
{
    // missing state file means app is OK
    QFile stateFile(stateFilePath);
    if (stateFile.exists()) {
        if (!stateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            WARN("Failed to open " << stateFilePath << ": " << stateFile.errorString());
            return;
        }

        QByteArray data = stateFile.readAll();
        stateFile.close();

        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject json(doc.object());
        JsonValidator jsonValidator(StateJsonDef);
        if (!jsonValidator.validate(json)) {
            // had errors
            DEBUG("Discarded statecfg " << stateFilePath << " as it had errors: " << jsonValidator.errors());
            application.markState(Application::Invalid);
        } else {
           QString state = json[JSON_STATE].toString();
            if (state == "valid") {
                application.markState(Application::Valid);
            } else if (state == "invalid") {
                application.markState(Application::Invalid);
            } else if (state == "downloading") {
                application.markState(Application::Downloading);
            } else {
                WARN("State file" << stateFilePath << "had unknown state:" << state);
                application.markState(Application::Invalid);
            }
        }
    } else {
        application.markState(Application::Valid);
    }
}

void ApplicationConfigReaderWriter::writeApplicationConfig(const QString &configFilePath, const Application &application)
{
    // TODO:
}

void ApplicationConfigReaderWriter::writeApplicationState(const QString &stateFilePath, const Application &application)
{
    // TODO:
}
