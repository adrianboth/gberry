#include "restinvocationdefinition.h"

namespace GBerry {

RESTInvocationDefinition::RESTInvocationDefinition()
{

}

RESTInvocationDefinition::~RESTInvocationDefinition()
{

}

RESTInvocationDefinition::HttpStatus RESTInvocationDefinition::resolveHttpStatus(int code)
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

RESTInvocationDefinition::HttpOperation RESTInvocationDefinition::httpOperation() const
{
    return _httpOperation;
}

void RESTInvocationDefinition::setHttpOperation(RESTInvocationDefinition::HttpOperation op)
{
    _httpOperation = op;
}

QString RESTInvocationDefinition::invocationPath() const
{
    return _invocationPath;
}

void RESTInvocationDefinition::setInvocationPath(const QString &invocationPath)
{
    _invocationPath = invocationPath;
}

} // eon
