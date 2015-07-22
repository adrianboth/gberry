#include "networkerrorreasons.h"

namespace GBerryLib {

NetworkErrorReasons::NetworkErrorReasons()
{

}

NetworkErrorReasons::~NetworkErrorReasons()
{

}

// this will now convert only errors that would be interest of end user
Result::Reason NetworkErrorReasons::from(QNetworkReply::NetworkError error)
{
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
        return Result::reasonFromCode("ConnectionRefused", QT_TRANSLATE_NOOP("Errors", "TXT_Connection refused to remote host"));

    case QNetworkReply::RemoteHostClosedError:
        return Result::reasonFromCode("RemoteHostClosedError", QT_TRANSLATE_NOOP("Errors", "TXT_Remote host closed connection before reply was received"));

    case QNetworkReply::HostNotFoundError:
        return Result::reasonFromCode("HostNotFoundError", QT_TRANSLATE_NOOP("Errors", "TXT_Remote host not found)"));

    case QNetworkReply::TimeoutError:
        return Result::reasonFromCode("TimeoutError", QT_TRANSLATE_NOOP("Errors", "TXT_Connection to remote host timed out"));

    case QNetworkReply::OperationCanceledError:
        return Result::reasonFromCode("OperationCanceledError", QT_TRANSLATE_NOOP("Errors", "TXT_Connection to remote host was cancelled locally before a reply was received"));

    case QNetworkReply::SslHandshakeFailedError:
        return Result::reasonFromCode("SslHandshakeFailedError", QT_TRANSLATE_NOOP("Errors", "TXT_Connection could not be established because encrypted connection creation failed"));

    case QNetworkReply::TemporaryNetworkFailureError:
        return Result::reasonFromCode("TemporaryNetworkFailureError", QT_TRANSLATE_NOOP("Errors", "TXT_Connection was broken as network connectivity was lost"));

    case QNetworkReply::NetworkSessionFailedError:
        return Result::reasonFromCode("NetworkSessionFailedError", QT_TRANSLATE_NOOP("Errors", "TXT_Connection failed because no working network connection available"));

    // QNetworkReply::BackgroundRequestNotAllowedError	9	the background request is not currently allowed due to platform policy.
    // QNetworkReply::ProxyConnectionRefusedError	101	the connection to the proxy server was refused (the proxy server is not accepting requests)
    // QNetworkReply::ProxyConnectionClosedError	102	the proxy server closed the connection prematurely, before the entire reply was received and processed
    // QNetworkReply::ProxyNotFoundError	103	the proxy host name was not found (invalid proxy hostname)
    // QNetworkReply::ProxyTimeoutError	104	the connection to the proxy timed out or the proxy did not reply in time to the request sent
    // QNetworkReply::ProxyAuthenticationRequiredError	105	the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)
    // QNetworkReply::ContentAccessDenied	201	the access to the remote content was denied (similar to HTTP error 401)
    // QNetworkReply::ContentOperationNotPermittedError	202	the operation requested on the remote content is not permitted
    // QNetworkReply::ContentNotFoundError	203	the remote content was not found at the server (similar to HTTP error 404)
    // QNetworkReply::AuthenticationRequiredError	204	the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)
    // QNetworkReply::ContentReSendError	205	the request needed to be sent again, but this failed for example because the upload data could not be read a second time.
    // QNetworkReply::ContentConflictError	206	the request could not be completed due to a conflict with the current state of the resource.
    // QNetworkReply::ContentGoneError	207	the requested resource is no longer available at the server.
    // QNetworkReply::InternalServerError	401	the server encountered an unexpected condition which prevented it from fulfilling the request.
    // QNetworkReply::OperationNotImplementedError	402	the server does not support the functionality required to fulfill the request.
    // QNetworkReply::ServiceUnavailableError	403	the server is unable to handle the request at this time.
    // QNetworkReply::ProtocolUnknownError	301	the Network Access API cannot honor the request because the protocol is not known
    // QNetworkReply::ProtocolInvalidOperationError	302	the requested operation is invalid for this protocol
    // QNetworkReply::UnknownNetworkError	99	an unknown network-related error was detected
    // QNetworkReply::UnknownProxyError	199	an unknown proxy-related error was detected
    // QNetworkReply::UnknownContentError	299	an unknown error related to the remote content was detected
    // QNetworkReply::ProtocolFailure	399	a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)
    // QNetworkReply::UnknownServerError	499	an unknown error related to the server response was detected

    default:
        return Result::reasonFromCode("QNetworkReplyError", QT_TRANSLATE_NOOP("Errors", "TXT_Encountered undocumented network error"));
    }
}

} // eon
