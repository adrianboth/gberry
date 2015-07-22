#include "invocation.h"

namespace GBerry {

const InvocationError InvocationErrors::UNSPECIFIED( 1000, "UNSPECIFIED",  QT_TRANSLATE_NOOP("Errors", "TXT_Invocation connection failed"));
const InvocationError InvocationErrors::CONNECTION_FAILED( 1100, "CONNECTION_FAILED",  QT_TRANSLATE_NOOP("Errors", "TXT_Invocation connection failed"));
const InvocationError InvocationErrors::CONNECTION_TIMEOUT(1101, "CONNECTION_TIMEOUT", QT_TRANSLATE_NOOP("Errors", "TXT_Invocation connection timed out"));
const InvocationError InvocationErrors::INVOCATION_FAILED( 1200, "INVOCATION_FAILED",  QT_TRANSLATE_NOOP("Errors", "TXT_Invocation failed"));
const InvocationError InvocationErrors::INVOCATION_INVALID(1201, "INVOCATION_INVALID", QT_TRANSLATE_NOOP("Errors", "TXT_Invocation parameters are invalid"));

Invocation::Invocation(QObject *parent) : QObject(parent)
{
}

Invocation::~Invocation()
{
}

} // eon
