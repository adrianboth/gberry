#ifndef UTIL_TESTDATA_H
#define UTIL_TESTDATA_H

#include <QDir>

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s

namespace TestUtils
{
QString testdataDirPath(QString caseName)
{
    QDir testdataRoot = QDir(xstr(TESTDATA_ROOT));
    return testdataRoot.filePath(caseName);
}
} // eon
#endif // UTIL_TESTDATA_H

