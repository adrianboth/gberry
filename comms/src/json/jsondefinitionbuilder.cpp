#include "jsondefinitionbuilder.h"

JsonDefinitionBuilder::JsonDefinitionBuilder()
{

}

JsonDefinitionBuilder::~JsonDefinitionBuilder()
{

}

IJsonMemberValidator& JsonDefinitionBuilder::hasStringMember(const QString& memberName)
{
    JsonStringMemberValidator* validator = new JsonStringMemberValidator(memberName);
    _validators.append(validator);
    return *validator;
}

IJsonMemberValidator& JsonDefinitionBuilder::hasOptionalStringMember(const QString& memberName)
{
    JsonStringMemberValidator* validator = new JsonStringMemberValidator(memberName, true); // optional=true
    _validators.append(validator);
    return *validator;
}

IJsonMemberValidator& JsonDefinitionBuilder::hasBooleanMember(const QString& memberName)
{
    JsonBooleanMemberValidator* validator = new JsonBooleanMemberValidator(memberName);
    _validators.append(validator);
    return *validator;
}

IJsonMemberValidator& JsonDefinitionBuilder::hasOptionalBooleanMember(const QString& memberName)
{
    JsonBooleanMemberValidator* validator = new JsonBooleanMemberValidator(memberName, true); // optional=true
    _validators.append(validator);
    return *validator;
}

QSharedPointer<JsonDefinition> JsonDefinitionBuilder::definition()
{
    return QSharedPointer<JsonDefinition>(new JsonDefinition(_validators));
}


// --
JsonStringMemberValidator::JsonStringMemberValidator(const QString &name, bool optional) :
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


// -----

JsonBooleanMemberValidator::JsonBooleanMemberValidator(const QString &name, bool optional) :
    _name(name),
    _optional(optional)
{
}

JsonBooleanMemberValidator::~JsonBooleanMemberValidator()
{
}

QStringList JsonBooleanMemberValidator::validate(QJsonObject &json)
{
    QStringList errors;
    if (!_optional && !json.contains(_name)) {
        errors << "Json object doesn't have member '" + _name + "'";

    // has member, but has right type?
    } else if (json.contains(_name) && !json[_name].isBool()) {
        errors << "Json object member '" + _name + "' is not boolean type";
    }

    return errors;
}

// TODO: could we have enum in json
// TODO: we could have Parser that maps read values into Object properties
