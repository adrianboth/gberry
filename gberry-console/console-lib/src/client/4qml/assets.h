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
 
#ifndef ASSETS_H
#define ASSETS_H

#include <QObject>

class Assets : public QObject
{
    Q_OBJECT
public:
    explicit Assets(QObject *parent = 0);
    ~Assets();

    void setRootPath(QString rootPath);

    Q_INVOKABLE QString filePath(const QString& fileRelPath) const;
    Q_INVOKABLE bool isValidFilePath(const QString& filePath) const;
    Q_INVOKABLE QString readAll(const QString& fileRelPath) const;

signals:

public slots:

private:
    QString _rootPath;
};

#endif // ASSETS_H
