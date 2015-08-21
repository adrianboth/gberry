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

#include "applicationexecutionsetup.h"

#include <utils/fileutils.h>
#include <QStringList>

namespace GBerryComms {

const ApplicationExecutionSetupError ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED(600, "APPLICATION_SETUP_FAILED");
const ApplicationExecutionSetupError ApplicationExecutionSetupErrors::LOGGING_SETUP_FAILED(601, "LOGGING_SETUP_FAILED");


class ApplicationExecutionSetup::Private {
public:

    QtLibrariesManager* qtlibsManager{nullptr};
    bool enableOutputLogging{false};
    ApplicationRegistry* registry{nullptr};
    QStringList additionalArgs;
    QMap<QString, QString> envVars;

    bool setupLogging(QProcess& process, const IApplication& app, Result& result) {
        if (enableOutputLogging) {
            process.setProcessChannelMode(QProcess::MergedChannels);
            QString logFilePath = GBerryLib::joinpath(
                        app.meta()->applicationDirPath(),
                        "output.log");

            // write header to log file and test that log file is writable
            QFile logFile(logFilePath);
            if (logFile.open(QIODevice::Append)) {
                // TODO: write timestamp and perhaps other info
                logFile.write("\n---------- LAUNCHING ----------\n");
                logFile.close();
                process.setStandardOutputFile(logFilePath, QIODevice::Append);

            } else {
                result << ApplicationExecutionSetupErrors::LOGGING_SETUP_FAILED
                       << Result::reasonFromDesc("Failed to open log for writing: #{file}")
                       << Result::Meta("file", logFilePath);
                return false;
            }

        } else {
            process.setProcessChannelMode(QProcess::ForwardedErrorChannel);
            process.setStandardOutputFile(QProcess::nullDevice());
        }

        return true;
    }
};

ApplicationExecutionSetup::ApplicationExecutionSetup(QtLibrariesManager *qtlibsManager, ApplicationRegistry *registry, bool enableOutputLogging) :
    _d(new Private)
{
    _d->qtlibsManager = qtlibsManager;
    _d->registry = registry;
    _d->enableOutputLogging = enableOutputLogging;
}

ApplicationExecutionSetup::~ApplicationExecutionSetup()
{

}

bool ApplicationExecutionSetup::prepare(QProcess &process, const IApplication &app, Result& result)
{
    // shell script
    // qtconf
    // lib paths (+qml)

    // TODO: this is needed only for Raspi as we know, but testing is easier if
    //       we use this also on desktop
    QFile runSh(GBerryLib::joinpath(app.meta()->applicationDirPath(), "run.sh"));
    if (!runSh.open(QIODevice::WriteOnly)) {
        result << ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED
               << Result::reasonFromDesc("Opening file #{file} for writing failed")
               << Result::Meta("file", runSh.fileName());

        return false;
    }

    // #!/bin/bash
    // THIS_DIR=`cd $(dirname $0) && pwd`
    // exec $THIS_DIR/tictactoe $*

    runSh.write("#!/bin/bash\nTHIS_DIR=`cd $(dirname $0) && pwd`\nexec ");
    runSh.write(app.meta()->applicationExecutablePath().toLatin1());
    runSh.write(" $*\n");
    runSh.close();

    // TODO: when we have a specific user for running apps, this might need to change
    if (!runSh.setPermissions(QFileDevice::ReadOwner|QFileDevice::ExeOwner|QFileDevice::WriteOwner)) {
        result << ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED
               << Result::reasonFromDesc("Couldn't modify file permissions for run.sh");
        return false;
    }

    // -----
    Result subresult;
    _d->qtlibsManager->createQtConf(app.meta()->applicationDirPath(), subresult);
    if (subresult.hasErrors()) {
        result << ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED
               << subresult;
        return false;
    }

    // -----
    QString appExe = app.meta()->applicationExecutablePath();
    if (!QFile::exists(appExe)) {
        result << ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED
               << Result::reasonFromDesc("Launch failed as requested application executable not exists: #{exe_path}")
               << Result::Meta("exe_path", app.meta()->applicationExecutablePath());
        return false;
    }

    process.setProgram(runSh.fileName());
    process.setWorkingDirectory(app.meta()->applicationDirPath());

    // -----
    Result subresult2;

    _d->setupLogging(process, app, subresult2);
    if (subresult2.hasErrors()) {
        result << ApplicationExecutionSetupErrors::EXECUTION_SETUP_FAILED
               << subresult;
        return false;
    }

    // -----
    QStringList args(_d->additionalArgs);

    if (_d->registry) {
        // registry is used to identify who is making TCP connections
        args << "--application-code=" + _d->registry->createIdentificationCode(app.meta()->applicationId());  
    }
    process.setArguments(args);

    QStringList envs(QProcess::systemEnvironment());
    foreach(QString key, _d->envVars.keys()) {
        envs << QString(key + "=" + _d->envVars[key]);
    }
    process.setEnvironment(envs);
    return true;
}

void ApplicationExecutionSetup::setAdditionalArguments(const QStringList &args)
{
    _d->additionalArgs = args;
}

void ApplicationExecutionSetup::setEnvironmentVariable(const QString &varName, const QString &varValue)
{
    _d->envVars[varName] = varValue;
}

} // eon
