#ifndef UIAPPSTATEMACHINE_PRIVATE_H
#define UIAPPSTATEMACHINE_PRIVATE_H

// This file contains private implementation of UIAppStateMachine

#include <QObject>
#include <QState>
#include <QDebug>


static const char* STATE_STARTUP = "startup";
static const char* STATE_WAITAPP_LAUNCHING_MAINUI = "waitAppVisibleLaunchingMainUI";
static const char* STATE_MAINUI = "mainuiVisible";
static const char* STATE_WAITAPP_LAUNCHING_APP = "waitAppVisibleLaunchingApp";
static const char* STATE_APP= "appVisible";

class UIAppStateMachinePrivate : public QObject
{
    Q_OBJECT

public:
    void emitAppLaunchRequested() { emit appLaunchRequested(); }
    void emitAppExitRequested()   { emit appExitRequested(); }

signals:
    void appLaunchRequested();
    void appExitRequested();

private:

};


class State : public QState
{
    Q_OBJECT

public:
    State(QString name = "") : QState(), _stateName(name) {}
    ~State() {}

    QString name() const { return _stateName; }

signals:
    void entered();
    void enteredState(QString stateName);

protected:
    virtual void onEntry(QEvent* event) { qDebug() << "onEntry: " << _stateName; Q_UNUSED(event); emit entered(); emit enteredState(_stateName); }

private:
    QString _stateName;

};

#endif // UIAPPSTATEMACHINE_PRIVATE_H

