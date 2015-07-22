#ifndef RESULTMESSAGEFORMATTER_H
#define RESULTMESSAGEFORMATTER_H

#include <QJsonObject>

#include "result.h"

namespace GBerryLib {

class ResultMessageFormatter
{
public:
    explicit ResultMessageFormatter(const Result& result);
    ~ResultMessageFormatter();

    QString createDeveloperMessage() const;
    QString createEndUserMessage() const; // TODO: localized

    // JSON format:
    //
    // result_details
    //   [error_string]
    //   [errors]
    //     * [meta]
    //   [meta]
    //   [reason]
    //   [subresult]

    QJsonObject toJson() const;

private:
    Result _result;
};

} // eon

#endif // RESULTMESSAGEFORMATTER_H
