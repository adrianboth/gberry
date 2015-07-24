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
 
 #ifndef SYSTEMSERVICES_H
#define SYSTEMSERVICES_H

#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include <QProcessEnvironment>



class SystemServices : public QObject
{
    Q_OBJECT

public:
    static const char* SERVICE_NAME;

    /**
     * @brief Provides singleshot timer
     *
     * Usage:
     *   singleshotTimer(PROCESS_KILL_WAIT_MS, this, SLOT(launch()));
     */
    virtual void singleshotTimer(int msec, QObject *receiver, const char *member)
    {
        Q_UNUSED(msec); Q_UNUSED(receiver); Q_UNUSED(member);
        qWarning("WARNING: Current instance of SystemServices doesn't implement singleshotTimer()");
    }

    static SystemServices * instance();

private:
    static SystemServices* __defaultInstance;
};

//const char* SystemServices::SERVICE_NAME = "systemservices";



class IEnvironmentVariables
{

public:
    IEnvironmentVariables() {}
    virtual ~IEnvironmentVariables() {}

    virtual bool contains(const QString& key) const = 0;
    virtual QString value(const QString& key, const QString& defaultValue = QString()) const = 0;
};


class EnvironmentVariables : public IEnvironmentVariables
{
public:
    EnvironmentVariables(QProcessEnvironment env);
    virtual ~EnvironmentVariables();

    virtual bool contains(const QString& key) const override;
    virtual QString value(const QString& key, const QString& defaultValue = QString()) const override;

private:
    QProcessEnvironment _env;
};

#endif // SYSTEMSERVICES_H

