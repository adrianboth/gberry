#ifndef APPLICATIONMETA_H
#define APPLICATIONMETA_H

#include <QObject>
#include <QString>
#include <QDebug>

#include "interfaces/iapplicationmeta.h"


class ApplicationMeta : public IApplicationMeta
{

public:
    ApplicationMeta() : _id("") {}
    virtual ~ApplicationMeta() { } //qDebug() << "ApplicationMeta destructor";}

    // format: <id>-<version>
    virtual QString id() const override { return _id; }

    virtual QString applicationId() const { return _applicationId; }
    virtual void setApplicationId(QString newValue) { _applicationId = newValue; }

    virtual QString name() const override { return _name; }
    virtual void setName(QString newValue) { _name = newValue; buildID(); }

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

private:
    void buildID() { _id = _applicationId + "-" + _version; }
    QString _id;

    QString _applicationId;
    ApplicationVersion _version;
    QString _name;
    QString _description;
    QString _applicationDirPath;
    QString _applicationExecutablePath;
    QString _catalogImageFilePath;
};

#endif // APPLICATIONMETA_H

