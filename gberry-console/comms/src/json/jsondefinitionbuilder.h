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
 
 #ifndef JSONDEFINITIONBUILDER_H
#define JSONDEFINITIONBUILDER_H

#include <QString>
#include <QStringList>
#include <QSharedPointer>

#include "jsondefinition.h"

/**
 * @brief The JsonDefinitionBuilder class
 */
class JsonDefinitionBuilder
{
public:
    JsonDefinitionBuilder();
    ~JsonDefinitionBuilder();

    IJsonMemberValidator& hasStringMember(const QString& memberName);
    IJsonMemberValidator& hasOptionalStringMember(const QString& memberName);

    IJsonMemberValidator& hasBooleanMember(const QString& memberName);
    IJsonMemberValidator& hasOptionalBooleanMember(const QString& memberName);

    QSharedPointer<JsonDefinition> definition();

private:
    QList<IJsonMemberValidator*> _validators;
};

/**
 * @brief The JsonStringMemberValidator class
 */
class JsonStringMemberValidator : public IJsonMemberValidator
{
public:
    JsonStringMemberValidator(const QString& name, bool optional = false);
    virtual ~JsonStringMemberValidator();

    virtual QStringList validate(QJsonObject& json) override;

private:
    QString _name;
    bool _optional;
};

class JsonBooleanMemberValidator : public IJsonMemberValidator
{
public:
    JsonBooleanMemberValidator(const QString& name, bool optional = false);
    virtual ~JsonBooleanMemberValidator();

    virtual QStringList validate(QJsonObject& json) override;

private:
    QString _name;
    bool _optional;

};


#endif // JSONDEFINITIONBUILDER_H
