#ifndef CURRENTAPPLICATION_H
#define CURRENTAPPLICATION_H

#include <QObject>
#include <QJsonObject>

namespace GBerryApplication {

class CurrentApplication : public QObject
{
    Q_OBJECT

public:
    CurrentApplication();
    ~CurrentApplication();

    void setRootPath(const QString& rootPath);

    Q_INVOKABLE QString applicationName() const;
    Q_INVOKABLE QString applicationVersionString() const;

private:
    void readConfig();
    static QString findApplicationConfigFile(const QString& applicationDirPath);

    QString _rootPath;
    QJsonObject _appcfg;
};

} // eon

#endif // CURRENTAPPLICATION_H
