#include "httpinvocationdefinition.h"

namespace GBerry {

HTTPInvocationDefinition::HTTPInvocationDefinition()
{

}

HTTPInvocationDefinition::~HTTPInvocationDefinition()
{

}

HTTPInvocationDefinition::Status HTTPInvocationDefinition::resolveHttpStatus(int code)
{
    switch (code)
    {
    case 200:
        return OK_200;
    case 403:
        return FORBIDDEN_403;
    default:
        return UNDEFINED;
    }
}

HTTPInvocationDefinition::Operation HTTPInvocationDefinition::httpOperation() const
{
    return _httpOperation;
}

void HTTPInvocationDefinition::setHttpOperation(HTTPInvocationDefinition::Operation op)
{
    _httpOperation = op;
}

QString HTTPInvocationDefinition::invocationPath() const
{
    return _invocationPath;
}

void HTTPInvocationDefinition::setInvocationPath(const QString &invocationPath)
{
    _invocationPath = invocationPath;
}

} // eon
