#include "jsonutils.h"

#include <QJsonDocument>

#include "log/log.h"


JsonUtils::JsonUtils(const char* logArea)
{
    Q_UNUSED(logArea);
}

JsonUtils::~JsonUtils()
{

}

void JsonUtils::debugPrint(const QString& name, const QJsonObject& json)
{
    DEBUG("QJsonObject" << name << ":" << QJsonDocument(json).toJson());
}

void JsonUtils::debugPrint(const QString& name, const QJsonArray& json)
{
    DEBUG("QJsonArray" << name << ":" << QJsonDocument(json).toJson());
}
