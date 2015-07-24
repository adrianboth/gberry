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
