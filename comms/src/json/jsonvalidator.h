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
