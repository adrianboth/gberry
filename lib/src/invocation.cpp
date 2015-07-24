/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
