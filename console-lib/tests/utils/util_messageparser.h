#ifndef UTILMESSAGEPARSER_H
#define UTILMESSAGEPARSER_H

#include <QJsonObject>
#include <QJsonParseError>

class QByteArray;

class UtilMessageParser
{
public:
    UtilMessageParser();
    explicit UtilMessageParser(const QByteArray& msg);
    ~UtilMessageParser();

    QJsonParseError parserError;
    QJsonObject json;

    void parse(const QByteArray& msg);
    bool isOk();
    QString errorString();
    bool isCommand(QString cmdId);

};

#endif // UTILMESSAGEPARSER_H
