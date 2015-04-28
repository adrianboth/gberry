#ifndef ASSETS_H
#define ASSETS_H

#include <QObject>

class Assets : public QObject
{
    Q_OBJECT
public:
    explicit Assets(QObject *parent = 0);
    ~Assets();

    void setRootPath(QString rootPath);

    Q_INVOKABLE QString filePath(QString fileRelPath) const;

signals:

public slots:

private:
    QString _rootPath;
};

#endif // ASSETS_H
