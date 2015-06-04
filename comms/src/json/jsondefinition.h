#ifndef JSONDEFINITION_H
#define JSONDEFINITION_H

#include <QJsonObject>

#include "json/jsonvalidationresult.h"

class IJsonMemberValidator;

/**
 * @brief Defines expected structure of Json object
 *
 * JsonDefinition is used to define expect structure of QJsonObject.
 * Object doesn't keep dynamic state so it can be used as static
 * object to be shared between users. Use JsonValidator for convinient
 * recording of errors.
 */
class JsonDefinition
{
public:
    JsonDefinition(QList<IJsonMemberValidator*> validators);
    ~JsonDefinition();

    // return true if valid
    bool validate(QJsonObject& json, JsonValidationResult& result) const;

private:
    QList<IJsonMemberValidator*> _validators;

};


class IJsonMemberValidator
{
public:
    IJsonMemberValidator() {}
    virtual ~IJsonMemberValidator() {}
    virtual QStringList validate(QJsonObject& json) = 0;
};


#endif // JSONDEFINITION_H
