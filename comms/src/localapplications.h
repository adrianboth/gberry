#ifndef LOCALAPPLICATIONS_H
#define LOCALAPPLICATIONS_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "interfaces/iapplicationmetas.h"
#include "interfaces/iapplicationsstorage.h"


class LocalApplications : public QObject, public IApplicationMetas
{
    Q_OBJECT
public:
    explicit LocalApplications(IApplicationsStorage* storage, QObject *parent = 0);
    ~LocalApplications();

    virtual QList<QSharedPointer<ApplicationMeta>> applications() const override;
    virtual QSharedPointer<ApplicationMeta> application(const QString& uniqueID) const override;
    virtual QList<QSharedPointer<ApplicationMeta>> applicationsByApplicationId(const QString& applicationId) const override;

signals:

public slots:

private slots:
    void onApplicationsUpdated();

private:
    IApplicationsStorage* _storage;
    QMap<QString, QSharedPointer<ApplicationMeta>> _apps;

};

#endif // LOCALAPPLICATIONS_H
