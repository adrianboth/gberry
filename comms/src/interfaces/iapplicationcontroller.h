#ifndef IAPPLICATIONCONTROLLER_H
#define IAPPLICATIONCONTROLLER_H

#include <QObject>

class IApplicationController : public QObject
{
    Q_OBJECT

public:
    IApplicationController(QObject* parent = 0) : QObject(parent) {}
    ~IApplicationController() {}

    virtual void launch() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    virtual QString fullApplicationId() const = 0;

signals:
    void launched();
    void launchFailed();
    void resumed();
    void paused();
    void resumeFailed();
    void stopped();
    void died();
};

#endif // IAPPLICATIONCONTROLLER_H
