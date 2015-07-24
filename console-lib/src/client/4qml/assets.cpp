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
