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
 
 #include "systemservices.h"

const char* SystemServices::SERVICE_NAME = "systemservices";

SystemServices* SystemServices::__defaultInstance = new SystemServices();


SystemServices* SystemServices::instance()
{
    QCoreApplication* app = QCoreApplication::instance();
    QVariant var = app->property(SERVICE_NAME);
    if (var.isValid() && var.canConvert<QObject*>())
    {
        QObject* obj = var.value<QObject*>();
        SystemServices* services = qobject_cast<SystemServices*>(obj);
        return services;
    }
    else
    {
        qWarning("WARNING: Couldn't find valid SystemServices");
        return __defaultInstance;
    }
    return NULL;
}

// ---

EnvironmentVariables::EnvironmentVariables(const QProcessEnvironment env) :
    _env(env)
{
}

EnvironmentVariables::~EnvironmentVariables()
{
}

bool EnvironmentVariables::contains(const QString &key) const
{
    return _env.contains(key);
}

QString EnvironmentVariables::value(const QString &key, const QString &defaultValue) const
{
    return _env.value(key, defaultValue);
}
