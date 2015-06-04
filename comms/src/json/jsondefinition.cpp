#include "jsondefinition.h"

JsonDefinition::JsonDefinition(QList<IJsonMemberValidator*> validators) :
    _validators(validators)
{
    // takes ownership of validators
}

JsonDefinition::~JsonDefinition()
{
    foreach(auto validator, _validators) {
        delete validator;
    }
}

bool JsonDefinition::validate(QJsonObject &json, JsonValidationResult &result) const
{
    foreach(auto validator, _validators) {
        QStringList errors = validator->validate(json);

        if (!errors.isEmpty()){
            // errors found
            result.appendErrors(errors);
        }
    }

    return result.errors().isEmpty();
}
