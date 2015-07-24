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
 
 #ifndef COMMSCONFIG_H
#define COMMSCONFIG_H

class QSettings;

#include "commsparameters.h"
using namespace Comms;

namespace GBerryComms {

/**
 * CommsConfig is a configuration class and its values are read form a
 * *.ini file.
 */
class CommsConfig
{
public:
    // parameters used to find config file
    CommsConfig(CommsParameters* parameters);
    ~CommsConfig();

    bool hasKey(const QString& key) const;
    QString value(const QString& key) const;
    QString value(const QString& key, const QString& defaultValue) const;

private:
    QSettings* _settings;
};

} // eon

#endif // COMMSCONFIG_H
