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

    // TODO: how error code?

    static bool writeApplicationConfig(const QString& configFilePath, const Application& application, Result& result);
    static bool writeApplicationState(const QString& stateFilePath, const Application& application, Result& result);
    static QString findApplicationConfigFile(const QString& applicationDirPath);

private:
    QString _applicationDir;
};

#endif // APPLICATIONCONFIGREADERWRITER_H
