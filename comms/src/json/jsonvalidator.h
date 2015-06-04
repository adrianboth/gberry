#ifndef JSONVALIDATOR_H
#define JSONVALIDATOR_H

#include <QSharedPointer>
#include <QJsonObject>

#include "jsondefinition.h"



/**
 * @brief Validates json document/object based on JsonDefinition
 *
 * JsonValidator is dynamic part of json validation. It uses static JsonDefinition
 * for validation and records possible errors.
 *
 */
class JsonValidator
{
public:
    JsonValidator(QSharedPointer<JsonDefinition> def);
    ~JsonValidator();

    // return true if not valid, and there are errors
    bool validate(QJsonObject& json);

    void useDefinition(QSharedPointer<JsonDefinition> def);

    QStringList errors() const { return _lastValidation.errors(); }

private:
    QSharedPointer<JsonDefinition> _def;
    JsonValidationResult _lastValidation;

};


#endif // JSONVALIDATOR_H
