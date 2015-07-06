#ifndef RESTINVOCATIONDEFINITION_H
#define RESTINVOCATIONDEFINITION_H

#include <QString>

namespace GBerry {

class RESTInvocationDefinition
{
public:
    explicit RESTInvocationDefinition();
    ~RESTInvocationDefinition();

    enum HttpOperation { NOT_DEFINED, GET, POST };

    enum HttpStatus {
        UNDEFINED = 0,
        OK_200 = 200
    };

    HttpOperation httpOperation() const;
    void setHttpOperation(HttpOperation op);

    QString invocationPath() const;
    void setInvocationPath(const QString& invocationPath);

private:
    HttpOperation _httpOperation{NOT_DEFINED};
    QString _invocationPath;

};

} // eon

#endif // RESTINVOCATIONDEFINITION_H
