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
 
 #ifndef APPLICATIONCONFIGREADERWRITER_H
#define APPLICATIONCONFIGREADERWRITER_H

#include "application.h"

class ApplicationConfigReaderWriter
{
public:
    explicit ApplicationConfigReaderWriter(const QString& applicationDir);
    ~ApplicationConfigReaderWriter();

    class Result {
    public:
        enum Error {
            NoError,
            FileNotWritable,
            FileWriteFailed,
            InvalidJsonContent,
            UnknownState,
            ApplicationDirCreationFailed
        };

        Error error{NoError};
        // textual information for developer, not to be localized as such
        QString errorString;

        bool hasError() const { return error != NoError; }

        // returns false if there are errors
        bool record(Error error_, const QString& errorString_) {
            error = error_;
            errorString = errorString_;
            return !hasError();
        }

    };


    QSharedPointer<Application> readApplication();
    bool writeApplication(const Application& application, Result& result);

    // TODO: how error code?
    static QSharedPointer<Application> readApplicationConfig(const QString& configFilePath);
    static void readApplicationState(const QString& stateFilePath, Application& application);
    static void readApplicationMeta(const QString& metaFilePath, Application& application);

    // TODO: how error code?

    static bool writeApplicationConfig(const QString& configFilePath, const Application& application, Result& result);
    static bool writeApplicationState(const QString& stateFilePath, const Application& application, Result& result);
    static bool writeApplicationMeta(const QString& metaFilePath, const Application& application, Result& result);
    static QString findApplicationConfigFile(const QString& applicationDirPath);

private:
    QString _applicationDir;
};

#endif // APPLICATIONCONFIGREADERWRITER_H
