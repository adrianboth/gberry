#ifndef LOCALAPPLICATIONS_H
#define LOCALAPPLICATIONS_H

#include <QObject>
#include <QPointer>
#include <QMap>

#include "iapplications.h"
#include "localapplicationsstorage.h"


class LocalApplications : public QObject, public IApplications
{
    Q_OBJECT
public:
    explicit LocalApplications(LocalApplicationsStorage* storage, QObject *parent = 0);
    ~LocalApplications();

    virtual QList<QPointer<ApplicationMeta>> applications() const;
    virtual QPointer<ApplicationMeta> application(QString appID) const;

signals:

public slots:

private slots:
    void onApplicationsUpdated();

private:
    LocalApplicationsStorage* _storage;
    QMap<QString, QPointer<ApplicationMeta>> _apps;

};

#endif // LOCALAPPLICATIONS_H
