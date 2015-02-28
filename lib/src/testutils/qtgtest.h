#ifndef QTGTEST_H
#define QTGTEST_H

#include <ostream>
#include <QString>

// It's important that the << operator is defined in the SAME
// namespace that defines Bar.  C++'s look-up rules rely on that.
::std::ostream& operator<<(::std::ostream& os, const QString& bar);

#endif // QTGTEST_H

