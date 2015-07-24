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

    static const char* DEFAULT_APPLICATION_CONFIG_FILE_NAME = "appcfg.json";
    static const char* APPLICATION_STATE_FILE_NAME = "state.json";
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

QString ApplicationConfigReaderWriter::findApplicationConfigFile(const QString& applicationDirPath)
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

QSharedPointer<Application> ApplicationConfigReaderWriter::readApplication()
{
    QDir appDir(_applicationDir);

    QString cfgFilePath = findApplicationConfigFile(_applicationDir);
    if (cfgFilePath.isEmpty())
        return QSharedPointer<Application>(nullptr);

    QSharedPointer<Application> app(readApplicationConfig(cfgFilePath));

    QString stateFilePath = appDir.filePath("state.json");
    readApplicationState(stateFilePath, *app.data());

    return app;
}

bool ApplicationConfigReaderWriter::writeApplication(const Application &application, Result& result)
{
    QDir appDir(_applicationDir);
    QString cfgFilePath;

    if (appDir.exists()) {
        // we are overwriting
        // we allow appcfg.json to have different prefixes
        //  -> look for file
         cfgFilePath = findApplicationConfigFile(_applicationDir);

        // if cfg file not found fall back to default
        if (cfgFilePath.isEmpty()) {
            cfgFilePath = appDir.filePath(DEFAULT_APPLICATION_CONFIG_FILE_NAME);
        }

    } else {
        // we are creating new configuration
        if (!appDir.mkpath(appDir.absolutePath()))
            return result.record(Result::ApplicationDirCreationFailed, QString("Failed to create %1").arg(appDir.absolutePath()));

        cfgFilePath = appDir.filePath(DEFAULT_APPLICATION_CONFIG_FILE_NAME);
    }

    if (writeApplicationConfig(cfgFilePath, application, result)) {
        QString stateFilePath = appDir.filePath(APPLICATION_STATE_FILE_NAME);
        writeApplicationState(stateFilePath, application, result);
    }

    // TODO: error situation is not clear, should we delete everything or what

    return !result.hasError();
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

bool ApplicationConfigReaderWriter::writeApplicationConfig(const QString &configFilePath, const Application &application, Result& result)
{
    QFile file(configFilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return result.record(Result::FileNotWritable, QString("Failed to open %1").arg(configFilePath));

    // TODO: is this unnecessary check
    if (!file.isWritable())
        return result.record(Result::FileNotWritable, QString("File %1 not writable").arg(configFilePath));

    QSharedPointer<IApplicationMeta> meta = application.meta();
    QJsonObject json;

    json[JSON_APPCFG_ID] = meta->applicationId();
    json[JSON_APPCFG_NAME] = meta->name();
    json[JSON_APPCFG_VERSION] = meta->version();
    json[JSON_APPCFG_DESCRIPTION] = meta->description();
    json[JSON_APPCFG_APPLICATIONEXE] = meta->applicationExecutablePath();
    json[JSON_APPCFG_CATALOGIMAGE] = meta->catalogImageFilePath();
    json[JSON_APPCFG_SYSTEMAPP] = meta->isSystemApp();

    QJsonDocument jdoc(json);

    // validate just to check that we can read it later
    JsonValidator jsonValidator(JsonDef);
    if (!jsonValidator.validate(json)) {
        TRACE("Configuration data:" << jdoc.toJson());
        return result.record(Result::InvalidJsonContent, QString("Created configuration is not valid. Aborted writing. Validation errors: %1").arg(jsonValidator.errors().join(", ")));
    }

    if (file.write(jdoc.toJson()) == -1) {
        return result.record(Result::FileWriteFailed, QString("Writing to file %1 failed").arg(configFilePath));
    }

    file.close();
    return true;
}

bool ApplicationConfigReaderWriter::writeApplicationState(const QString &stateFilePath, const Application &application, Result& result)
{
    QFile file(stateFilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return result.record(Result::FileNotWritable, QString("Failed to open %1").arg(stateFilePath));

    // TODO: is this check needed?
    if (!file.isWritable())
        return result.record(Result::FileNotWritable, QString("File %1 not writable").arg(stateFilePath));

    QJsonObject json;

    switch (application.state()) {
    case Application::Valid:
        json[JSON_STATE] = "valid";
        break;
    case Application::Invalid:
        json[JSON_STATE] = "invalid";
        break;
    case Application::Downloading:
        json[JSON_STATE] = "downloading";
        break;
    default:
        return result.record(Result::UnknownState, "Application has unknown state");
    }

    QJsonDocument jdoc(json);

    // validate just to check that we can read it later
    JsonValidator jsonValidator(StateJsonDef);
    if (!jsonValidator.validate(json)) {
        TRACE("State data:" << jdoc.toJson());
        return result.record(Result::InvalidJsonContent, QString("Created state configuration is not valid. Aborted writing. Validation errors: %1").arg(jsonValidator.errors().join(", ")));
    }

    if (file.write(jdoc.toJson()) == -1) {
        return result.record(Result::FileWriteFailed, QString("Writing to file %1 failed").arg(stateFilePath));
    }

    file.close();
    return true;
}
