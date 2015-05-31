#ifndef APPLICATIONMETA_H
#define APPLICATIONMETA_H

#include <QObject>

class ApplicationMeta : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString applicationPath READ applicationPath)

public:
    ApplicationMeta(QString id, QObject* parent = 0) : QObject(parent), _id(id), _applicationPath("") {}
    ApplicationMeta(QString id, QString applicationPath, QObject* parent = 0) : QObject(parent), _id(id), _applicationPath(applicationPath) {}
    ~ApplicationMeta() {}

    QString id() const { return _id; }
    QString applicationPath() const { return _applicationPath; }

private:
    QString _id;
    QString _applicationPath;
};

#endif // APPLICATIONMETA_H

