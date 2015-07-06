#include "restinvocationdefinition.h"

namespace GBerry {

RESTInvocationDefinition::RESTInvocationDefinition()
{

}

RESTInvocationDefinition::~RESTInvocationDefinition()
{

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
