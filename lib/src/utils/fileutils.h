#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QDir>

namespace GBerryLib
{
QString joinpath (QString appDir, QString path) {
    return QDir(appDir).absoluteFilePath(path);
}

} // eon

#endif // FILEUTILS_H

