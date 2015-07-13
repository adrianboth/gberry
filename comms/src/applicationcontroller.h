#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QScopedPointer>

#include "interfaces/iapplicationcontroller.h"
#include "server/application/iapplication.h"
#include "server/applicationregistry.h"

using namespace GBerry::Console::Server;

class ApplicationControllerPrivate;

class ApplicationController : public IApplicationController
{
    Q_OBJECT

public:
    explicit ApplicationController(
            QSharedPointer<IApplication> app,
            ApplicationRegistry* registry,
            QObject *parent = 0);
    ApplicationController(QObject *parent = 0);
    ~ApplicationController();

    // IApplicationController
    virtual void launch() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void stop() override;
    // TODO: this refers to full-id (name + version), could there be better name
    virtual QString fullApplicationId() const override;

    // --
    virtual void setApplication(QSharedPointer<IApplication> app);
    virtual QSharedPointer<IApplication> application();
    virtual void enableSimulatedMode(bool enabled);

    virtual void enableOutputLogging(bool enabled);

    // -- dynamic properties
    static const char* PROCESS_KILL_WAIT_MS_PROP;

signals:

public slots:

private:
    const QScopedPointer<ApplicationControllerPrivate> _d;

};


#endif // APPLICATIONCONTROLLER_H
