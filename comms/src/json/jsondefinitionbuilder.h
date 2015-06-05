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
