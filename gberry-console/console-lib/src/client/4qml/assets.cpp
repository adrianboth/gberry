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
 
#include "assets.h"

#include <QProcessEnvironment>
#include <QDir>
#include <QDebug>
#include <QString>

#define LOG_AREA "Assets"
#include "log/log.h"

Assets::Assets(QObject *parent) : QObject(parent)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("GBERRY_APPLICATION_ASSETS_ROOT")) {
        _rootPath = env.value("GBERRY_APPLICATION_ASSETS_ROOT");
    }
    else
    {
        // we assume that application directory is the current dir
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

QString Assets::filePath(const QString& fileRelPath) const
{
    QString checkPath(fileRelPath); // not necessarely rel path ...
    QString newPath("file:");

    if (checkPath.startsWith("file:")) {
        checkPath.remove(0, 5);
    }

    if (!QFileInfo(checkPath).isAbsolute()) {
        newPath.append(_rootPath).append("/").append(checkPath);
    } else {
        newPath.append(checkPath);
    }

    return newPath;
}

bool Assets::isValidFilePath(const QString& filePath) const
{
    QString path = this->filePath(filePath).remove(0, 5); // remove file:
    QFileInfo f(path);
    return f.exists() && f.isFile();
}

QString Assets::readAll(const QString& fileRelPath) const
{
    QString path = this->filePath(fileRelPath).remove(0, 5); // remove file:
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        ERROR("Failed to read" << path);
        return "";
    }
    QString data = file.readAll();
    file.close();
    return data;
}
