#include "localapplicationsstorage.h"

#include <QSharedPointer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "json/jsonvalidator.h"
#include "json/jsondefinition.h"
#include "json/jsondefinitionbuilder.h"

#define LOG_AREA "LocalApplicationsStorage"
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

class LocalApplicationsStoragePrivate
{

public:
    LocalApplicationsStoragePrivate() {
        if (jsonDef.isNull()) {
            JsonDefinitionBuilder builder;

            builder.hasStringMember(JSON_APPCFG_ID);
            builder.hasStringMember(JSON_APPCFG_DESCRIPTION);
            builder.hasStringMember(JSON_APPCFG_NAME);
            builder.hasOptionalStringMember(JSON_APPCFG_CATALOGIMAGE);
            builder.hasOptionalStringMember(JSON_APPCFG_APPLICATIONEXE);
            builder.hasOptionalStringMember(JSON_APPCFG_DEVELOPERCOMMENTS);

            builder.hasOptionalBooleanMember(JSON_APPCFG_SYSTEMAPP);

            jsonDef = builder.definition();
        }

        if (stateJsonDef.isNull()) {
            JsonDefinitionBuilder builder;
            builder.hasStringMember(JSON_STATE);
            stateJsonDef = builder.definition();
        }
    }

    QDir appsDir;

    static QSharedPointer<JsonDefinition> jsonDef;
    static QSharedPointer<JsonDefinition> stateJsonDef;
};

QSharedPointer<JsonDefinition> LocalApplicationsStoragePrivate::jsonDef(NULL);
QSharedPointer<JsonDefinition> LocalApplicationsStoragePrivate::stateJsonDef(NULL);

LocalApplicationsStorage::LocalApplicationsStorage(QString appsDir, QObject *parent) :
    IApplicationsStorage(parent)
{
    _priv = new LocalApplicationsStoragePrivate();
    _priv->appsDir = appsDir;

    if (!_priv->appsDir.exists()) {
        WARN("Apps directory doesn't exist");
    }
}

LocalApplicationsStorage::~LocalApplicationsStorage()
{
    delete _priv;
}

QList<QSharedPointer<Application>> LocalApplicationsStorage::applications()
{
    QList<QSharedPointer<Application>> apps;

    // iterate folders in apps dir
    // read *appcfg.json from each folder

    QStringList dirs = _priv->appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList appcfgNameFilter;
    appcfgNameFilter << "*appcfg.json";

    foreach(QString dirName, dirs) {
        QDir d(_priv->appsDir.filePath(dirName));
        d.setNameFilters(appcfgNameFilter);
        QStringList cfgFiles = d.entryList(QDir::Files);
        if (cfgFiles.length() == 0) {
            DEBUG("Read" << d.path() << "but didn't found *appcfg.json");
            continue;
        } else if (cfgFiles.length() > 1) {
            WARN("Application directory" << d.path() << "had several config files. Will select one:" << cfgFiles[0]);
        }
        QString cfgFilePath = d.filePath(cfgFiles[0]);

        QFile file(cfgFilePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            WARN("Failed to open " << cfgFilePath << ": " << file.errorString());
            continue;
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
        JsonValidator jsonValidator(_priv->jsonDef);
        if (!jsonValidator.validate(json)) {
            // had errors
            DEBUG("Discarded appcfg " << cfgFilePath << " as it had errors: " << jsonValidator.errors());
            continue;
        }

        QSharedPointer<ApplicationMeta> meta(new ApplicationMeta);
        QSharedPointer<Application> app(new Application(meta));

        auto readStr = [&] (const char* key) { return json[key].toString(); };
        auto readBool = [&] (const char* key) { return json[key].toBool(); };

        meta->setApplicationId(readStr(JSON_APPCFG_ID));
        meta->setName(readStr(JSON_APPCFG_NAME));
        meta->setVersion(readStr(JSON_APPCFG_VERSION));
        meta->setDescription(readStr(JSON_APPCFG_DESCRIPTION));
        meta->setApplicationDirPath(d.absolutePath());

        if (json.contains(JSON_APPCFG_APPLICATIONEXE)) {
            meta->setApplicationExecutablePath(d.absoluteFilePath(readStr(JSON_APPCFG_APPLICATIONEXE)));
        } else {
            meta->setApplicationExecutablePath(d.absoluteFilePath(meta->applicationId()));
        }

        if (json.contains(JSON_APPCFG_CATALOGIMAGE)) {
            meta->setCatalogImageFilePath(d.absoluteFilePath(readStr(JSON_APPCFG_CATALOGIMAGE)));
        }

        if (json.contains(JSON_APPCFG_SYSTEMAPP)) {
            meta->setIsSystemApp(readBool(JSON_APPCFG_SYSTEMAPP));
        }

        // TODO: id in cfg in nicely mathcing to id() we are using
        // TODO: optional with default value
        //   * builder using def
        // TODO: optional checking type

        // TODO: rest setters

        QString stateFilePath = d.filePath("state.json");
        // missing state file means app is OK
        QFile stateFile(stateFilePath);
        if (stateFile.exists()) {
            if (!stateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                WARN("Failed to open " << stateFilePath << ": " << stateFile.errorString());
                continue;
            }

            QByteArray data = stateFile.readAll();
            stateFile.close();

            QJsonDocument doc(QJsonDocument::fromJson(data));
            QJsonObject json(doc.object());
            JsonValidator jsonValidator(_priv->stateJsonDef);
            if (!jsonValidator.validate(json)) {
                // had errors
                DEBUG("Discarded statecfg " << stateFilePath << " as it had errors: " << jsonValidator.errors());
                app->markState(Application::Invalid);
            } else {
               QString state = json[JSON_STATE].toString();
                if (state == "valid") {
                    app->markState(Application::Valid);
                } else if (state == "invalid") {
                    app->markState(Application::Invalid);
                } else if (state == "downloading") {
                    app->markState(Application::Downloading);
                } else {
                    WARN("State file" << stateFilePath << "had unknown state:" << state);
                    app->markState(Application::Invalid);
                }
            }

        } else {
            app->markState(Application::Valid);
        }

        DEBUG("Application" << app->id() << "read successfully");
        apps.append(app);
    }

    return apps;
}
