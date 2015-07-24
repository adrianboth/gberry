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
 
 #include "iapplication.h"

#define LOG_AREA "IApplication"
#include "log/log.h"

namespace GBerry {
    namespace Console {
        namespace Server {

IApplication::IApplication()
{
}

IApplication::~IApplication()
{
}

QString IApplication::stateString() const
{
    switch (state()) {
    case Valid:
        return "valid";
    case Invalid:
        return "invalid";
    case Downloading:
        return "downloading";

    default:
        ERROR("Unknown application state" << state() << ". Can't convert to string.");
        return "";
    }
}

QString IApplication::id() const
{
    return meta()->applicationId() + "-" + meta()->version();
}

}}} // eon

