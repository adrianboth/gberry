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
 
 #ifndef REALSYSTEMSERVICES_H
#define REALSYSTEMSERVICES_H

#include "systemservices.h"

class RealSystemServices : public SystemServices
{
public:
    RealSystemServices();
    ~RealSystemServices();

    virtual void singleshotTimer(int msec, QObject *receiver, const char *member);

    void registerItself();
};

#endif // REALSYSTEMSERVICES_H
