#ifndef UIAPPSTATEMACHINE_H
#define UIAPPSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QDebug>

#include "iapplicationcontroller.h"
#include "ilaunchcontroller.h"

class UIAppStateMachinePrivate;

/**
 * This takes care of starting waiting screen, mainui, and requested application.
 *
 * It setups necessary watchdogs to monitor that applications are running
 */
class UIAppStateMachine : public QObject
{
    Q_OBJECT

public:
    UIAppStateMachine(IApplicationController* waitApp,
                      IApplicationController* mainui,
                      ILaunchController* currentApp);

    ~UIAppStateMachine();


    void start();

    // TODO: this should be actually a class presenting applications
    void lauchApplication(QString applicationID);
    void exitCurrentApplication();

    // TODO: actually I need a generic ApplicationController
        // pure abstract to have interface to test
        // would have normal exit, yms,
        // how about crashing

    // TODO: how to signal error situations for user

    // utility for testing/debugging
    QString debugCurrentStateName() const;

public slots:
    void applicationConnectionValidated();

private:
    QStateMachine _stateMachine;
    IApplicationController* _waitapp;
    IApplicationController* _mainui;
    ILaunchController* _currentApp;

    UIAppStateMachinePrivate* _impl;
    QString _currentStateName;

};


#endif // UIAPPSTATEMACHINE_H
