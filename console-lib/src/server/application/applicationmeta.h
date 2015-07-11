#ifndef APPLICATIONMETA_H
#define APPLICATIONMETA_H

#include <QObject>
#include <QString>
#include <QDebug>

#include "server/application/iapplicationmeta.h"


namespace GBerry {
    namespace Console {
        namespace Server {


class ApplicationMeta : public IApplicationMeta
{

public:
    ApplicationMeta() : _id(""),_isSystemApp(false) {}
    ApplicationMeta(const IApplicationMeta& source) : _id(""),_isSystemApp(false) {
        setApplicationId(source.applicationId());
        setName(source.name());
        setVersion(source.version());
        setDescription(source.description());
        setApplicationDirPath(source.applicationDirPath());
        setApplicationExecutablePath(source.applicationExecutablePath());
        setCatalogImageFilePath(source.catalogImageFilePath());
        setIsSystemApp(source.isSystemApp());

        // TODO: how downloadable app has applicationdirpath, executable can catalogimage
        //   -- they don't except image ... but how it is transfered ...
        //   -- .. needs to be somehow downloaded separately

    }
    virtual ~ApplicationMeta() { } //qDebug() << "ApplicationMeta destructor";}

    // format: <id>-<version>
    virtual QString id() const override { return _id; }

    virtual QString applicationId() const { return _applicationId; }
    virtual void setApplicationId(QString newValue) { _applicationId = newValue; buildID(); }

    virtual QString name() const override { return _name; }
    virtual void setName(QString newValue) { _name = newValue; }

    virtual ApplicationVersion version() const override { return _version; }
    virtual void setVersion(ApplicationVersion newValue) { _version = newValue; buildID(); }

    virtual QString description() const override { return _description; }
    virtual void setDescription(QString newValue) { _description = newValue; }

    virtual QString applicationDirPath() const override { return _applicationDirPath; }
    virtual void setApplicationDirPath(QString newValue) { _applicationDirPath = newValue; }

    virtual QString applicationExecutablePath() const override { return _applicationExecutablePath; }
    virtual void setApplicationExecutablePath(QString newValue) { _applicationExecutablePath = newValue; }

    virtual QString catalogImageFilePath() const override { return _catalogImageFilePath; }
    virtual void setCatalogImageFilePath(QString newValue) { _catalogImageFilePath = newValue; }

    virtual bool isSystemApp() const override { return _isSystemApp; }
    virtual void setIsSystemApp(bool newValue) { _isSystemApp = newValue; }

private:
    // TODO: this could move to BaseApplication
    void buildID() { _id = _applicationId + "-" + _version; }
    QString _id;

    QString _applicationId;
    ApplicationVersion _version;
    bool _isSystemApp;
    QString _name;
    QString _description;
    QString _applicationDirPath;
    QString _applicationExecutablePath;
    QString _catalogImageFilePath;
};

}}} // eon

#endif // APPLICATIONMETA_H

