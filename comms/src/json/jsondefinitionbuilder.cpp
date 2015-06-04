#include "jsondefinitionbuilder.h"

JsonDefinitionBuilder::JsonDefinitionBuilder()
{

}

JsonDefinitionBuilder::~JsonDefinitionBuilder()
{

}

IJsonMemberValidator& JsonDefinitionBuilder::hasStringMember(QString memberName)
{
    JsonStringMemberValidator* validator = new JsonStringMemberValidator(memberName);
    _validators.append(validator);
    return *validator;
}

IJsonMemberValidator& JsonDefinitionBuilder::hasOptionalStringMember(QString memberName)
{
    JsonStringMemberValidator* validator = new JsonStringMemberValidator(memberName, true); // optional=true
    _validators.append(validator);
    return *validator;
}

QSharedPointer<JsonDefinition> JsonDefinitionBuilder::definition()
{
    return QSharedPointer<JsonDefinition>(new JsonDefinition(_validators));
}


// --
JsonStringMemberValidator::JsonStringMemberValidator(QString &name, bool optional) :
    _name(name),
    _optional(optional)
{
}

JsonStringMemberValidator::~JsonStringMemberValidator()
{
}

QStringList JsonStringMemberValidator::validate(QJsonObject &json)
{
    QStringList errors;
    if (!_optional && !json.contains(_name)) {
        errors << "Json object doesn't have member '" + _name + "'";

    // has member, but has right type?
    } else if (json.contains(_name) && !json[_name].isString()) {
        errors << "Json object member '" + _name + "' is not string type";
    }

    return errors;
}

// TODO: could we have enum in json
// TODO: we could have Parser that maps read values into Object properties
