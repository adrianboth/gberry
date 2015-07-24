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
