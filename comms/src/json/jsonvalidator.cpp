#include "jsonvalidator.h"

JsonValidator::JsonValidator(QSharedPointer<JsonDefinition> def) :
    _def(def)
{
}

JsonValidator::~JsonValidator()
{
}

void JsonValidator::useDefinition(QSharedPointer<JsonDefinition> def)
{
    _def = def;
    _lastValidation.clear();
}

bool JsonValidator::validate(QJsonObject& json)
{
    _lastValidation.clear();
    return _def->validate(json, _lastValidation);
}
