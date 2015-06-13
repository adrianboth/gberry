#include "util_messageparser.h"

#include <QJsonDocument>

UtilMessageParser::UtilMessageParser()
{
}

UtilMessageParser::UtilMessageParser(const QByteArray& msg)
{
    parse(msg);
}

UtilMessageParser::~UtilMessageParser()
{
}

void UtilMessageParser::parse(const QByteArray& msg)
{
    QJsonDocument doc(QJsonDocument::fromJson(msg, &parserError));
    if (isOk()) {
        json = doc.object();
    }
}

bool UtilMessageParser::isOk()
{
    return parserError.error == QJsonParseError::NoError;
}

QString UtilMessageParser::errorString()
{
    return parserError.errorString();
}

bool UtilMessageParser::isCommand(QString cmdId)
{
    return json.contains("command") && json["command"] == cmdId;
}
