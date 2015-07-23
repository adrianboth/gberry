#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QString>

#define ASSERT_QSTRING_EQ(first, second) ASSERT_TRUE(StringUtils::compareQStringsVerbose(first, second));

class StringUtils
{
public:
    StringUtils();
    ~StringUtils();

    static bool compareQStringsVerbose(const QString& first, const QString& second);
    // -1 means they are same
    static int findFirstNotCommonCharacter(const QString& first, const QString& second);
};

#endif // STRINGUTILS_H
