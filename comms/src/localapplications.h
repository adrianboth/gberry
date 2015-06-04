#ifndef LOCALAPPLICATIONS_H
#define LOCALAPPLICATIONS_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "interfaces/iapplications.h"
#include "localapplicationsstorage.h"


class LocalApplications : public QObject, public IApplications
{
    Q_OBJECT
public:
    explicit LocalApplications(LocalApplicationsStorage* storage, QObject *parent = 0);
    ~LocalApplications();

    virtual QList<QSharedPointer<ApplicationMeta>> applications() const;
    virtual QSharedPointer<ApplicationMeta> application(QString appID) const;

signals:

public slots:

private slots:
    void onApplicationsUpdated();

private:
    LocalApplicationsStorage* _storage;
    QMap<QString, QSharedPointer<ApplicationMeta>> _apps;

};

#endif // LOCALAPPLICATIONS_H
