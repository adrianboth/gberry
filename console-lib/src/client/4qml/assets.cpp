#include "assets.h"

#include <QProcessEnvironment>
#include <QDir>
#include <QDebug>
#include <QString>

Assets::Assets(QObject *parent) : QObject(parent)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("GBERRY_APPLICATION_ASSETS_ROOT")) {
        _rootPath = env.value("GBERRY_APPLICATION_ASSETS_ROOT");
    }
    else
    {
        _rootPath = QDir::current().absolutePath();
    }
    qDebug() << "Assets root path:" << _rootPath;
}

Assets::~Assets()
{

}

void Assets::setRootPath(QString rootPath)
{
    _rootPath = rootPath;
    qDebug() << "Assets root path set to" << _rootPath;
}

QString Assets::filePath(QString fileRelPath) const
{
    QString path ("file:");
    path.append(_rootPath).append("/").append(fileRelPath);

    return path;
}
