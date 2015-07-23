#include "stringutils.h"

#include <gtest/gtest.h>
#include <QDebug>

StringUtils::StringUtils()
{

}

StringUtils::~StringUtils()
{

}

bool StringUtils::compareQStringsVerbose(const QString &first, const QString &second)
{
    if (first.length() != second.length()) {
        qDebug() << "String have different size:" << first.length() << "vs" << second.length();
        int firstUncommon = findFirstNotCommonCharacter(first, second);
        qDebug() << "1:" << qPrintable(first);
        qDebug() << "2:" << qPrintable(second);
        if (firstUncommon != -1) {
            QString filler("^");
            qDebug() << "  " << qPrintable(filler.rightJustified(firstUncommon+1, ' '));
        }
        return false;
    }

    if (first != second) {
        qDebug() << "Strings are not equal";
        qDebug() << "1:" << qPrintable(first);
        qDebug() << "2:" << qPrintable(second);
        int firstUncommon = findFirstNotCommonCharacter(first, second);
        if (firstUncommon != -1) {
            QString filler("^");
            qDebug() << "  " << qPrintable(filler.rightJustified(firstUncommon+1, ' '));
        }
        return false;
    }
    return true;
}

int StringUtils::findFirstNotCommonCharacter(const QString &first, const QString &second)
{
    int minLength = first.length() < second.length() ? first.length() : second.length();

    for (int i = 0; i < minLength; i++) {
        if (first.at(i) != second.at(i)) {
            return i;
        }
    }

    return -1;
}

