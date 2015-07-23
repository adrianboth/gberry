#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <gtest/gtest.h>

#include "testutils/stringutils.h"

// use outside define LOG_AREA
#ifndef LOG_AREA
  #define LOG_AREA "JsonUtils"
#endif

#define ASSERT_JSON_STRING_VALUE(jsonObject, value) ASSERT_TRUE(StringUtils::compareQStringsVerbose(jsonObject.toString(), value))

class JsonUtils
{
public:
    JsonUtils(const char* logArea = LOG_AREA);
    ~JsonUtils();

    static void debugPrint(const QString& name, const QJsonObject& json);
    static void debugPrint(const QString& name, const QJsonArray& json);
};

#endif // JSONUTILS_H
