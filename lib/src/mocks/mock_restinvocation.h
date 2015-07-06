#ifndef MOCK_RESTINVOCATION_H
#define MOCK_RESTINVOCATION_H

#include "restinvocation.h"

#include <gmock/gmock.h>


class MockRESTInvocation : public RESTInvocation
{
 public:
     MOCK_METHOD1(defineGetOperation, void(const QString& path));
     MOCK_METHOD2(definePostOperation, void(const QString&  path, const QJsonDocument& jsondoc));
     MOCK_CONST_METHOD0(statusCode, RESTInvocation::InvocationStatus());
     MOCK_CONST_METHOD0(responseHttpStatusCode, RESTInvocationDefinition::HttpStatus());
     MOCK_CONST_METHOD0(responseAvailable, bool());
     MOCK_CONST_METHOD0(responseByteData, QByteArray());
     MOCK_CONST_METHOD0(responseString, QString());
     MOCK_CONST_METHOD0(errorString, QString());

     MOCK_METHOD0(execute, void());
     MOCK_METHOD0(abort, void());

     // no need to mock signals 'cos they don't have impl and they are not called
     // TODO: we could have a macro generate following kinds of functions
     void emitFinishedOK() { emit finishedOK(this); }
     void emitFinishedError() { emit finishedError(this); }
};

#endif // MOCK_RESTINVOCATION_H

