#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <client/clientsetup.h>

// trick to get define as a string
#define xstr(s) str(s)
#define str(s) #s


class ConsoleApplication : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleApplication(QObject *parent = 0);
    ~ConsoleApplication();

    void run(QString mainQmlUrl);

    // introduced in *.h to get fresh compiler defines
    void setImportPaths(QString importPathList)
    {
        // for debug builds we run app from builds/ directory and want use
        // qml file tree from source folders
        qDebug() << "Setting debug build qml import path:" << importPathList;

        // currently paths can't contain spaces
        foreach(auto path, importPathList.split(":")) {
            _engine.addImportPath(path);
        }
    };

signals:

public slots:

private:
    QQmlApplicationEngine _engine;
    ClientSetup _setup;



};

#endif // CONSOLEAPPLICATION_H
