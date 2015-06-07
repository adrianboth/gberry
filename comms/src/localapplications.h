#ifndef LOCALAPPLICATIONS_H
#define LOCALAPPLICATIONS_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "server/application/baseapplications.h"
#include "interfaces/iapplicationsstorage.h"

using namespace GBerry::Console::Server;

class LocalApplications : public QObject, public BaseApplications
{
    Q_OBJECT
public:
    explicit LocalApplications(IApplicationsStorage* storage, QObject *parent = 0);
    ~LocalApplications();

signals:

public slots:

private slots:
    void onApplicationsUpdated();

private:
    IApplicationsStorage* _storage;

};

#endif // LOCALAPPLICATIONS_H
