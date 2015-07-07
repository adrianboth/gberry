#ifndef APPLICATIONCONFIGREADERWRITER_H
#define APPLICATIONCONFIGREADERWRITER_H

#include "application.h"

class ApplicationConfigReaderWriter
{
public:
    explicit ApplicationConfigReaderWriter(const QString& applicationDir);
    ~ApplicationConfigReaderWriter();

    QSharedPointer<Application> readApplication();
    void writeApplication(const Application& application);

    // TODO: how error code?
    static QSharedPointer<Application> readApplicationConfig(const QString& configFilePath);
    static void readApplicationState(const QString& stateFilePath, Application& application);

    // TODO: how error code?

    static void writeApplicationConfig(const QString& configFilePath, const Application& application);
    static void writeApplicationState(const QString& stateFilePath, const Application& application);


private:
    QString _applicationDir;
};

#endif // APPLICATIONCONFIGREADERWRITER_H
