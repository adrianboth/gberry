#include "qtgtest.h"

// It's important that the << operator is defined in the SAME
// namespace that defines Bar.  C++'s look-up rules rely on that.
::std::ostream& operator<<(::std::ostream& os, const QString& bar) {
  return os << qPrintable(bar);
}

::std::ostream& operator<<(::std::ostream& os, const QStringList& bar) {
  return os << "[" << qPrintable(bar.join(", ")) << "]";
}
