#ifndef HTTPINVOCATIONDEFINITION_H
#define HTTPINVOCATIONDEFINITION_H

#include <QString>

namespace GBerry {

class HTTPInvocationDefinition
{
public:
    explicit HTTPInvocationDefinition();
    ~HTTPInvocationDefinition();

    enum Operation { NOT_DEFINED, GET, POST };

    enum Status {
        UNDEFINED = 0,
        OK_200 = 200,
        FORBIDDEN_403 = 403
    };

    static Status resolveHttpStatus(int code);

    Operation httpOperation() const;
    void setHttpOperation(Operation op);

    QString invocationPath() const;
    void setInvocationPath(const QString& invocationPath);

private:
    Operation _httpOperation{NOT_DEFINED};
    QString _invocationPath;

};

} // eon

#endif // HTTPINVOCATIONDEFINITION_H
