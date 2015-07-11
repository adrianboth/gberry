#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QDir>

namespace GBerryLib
{
QString joinpath (QString appDir, QString path);

//https://qt.gitorious.org/qt-creator/qt-creator/source/1a37da73abb60ad06b7e33983ca51b266be5910e:src/app/main.cpp#L13-189
// taken from utils/fileutils.cpp. We can not use utils here since that depends app_version.h.
bool copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath);

} // eon

#endif // FILEUTILS_H

