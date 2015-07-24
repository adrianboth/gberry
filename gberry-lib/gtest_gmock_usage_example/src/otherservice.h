#ifndef OTHERSERVICE_H
#define OTHERSERVICE_H

#include <QObject>
#include <QString>

#include "iotherservice.h"

// note that QObject needs to be first inherited
class OtherService : public QObject, public IOtherService
{
    Q_OBJECT
    Q_INTERFACES(IOtherService)
public:
    explicit OtherService(QObject *parent = 0);
    ~OtherService();

    void triggerOtherServiceSignal();

    // from IOtherService
    virtual QString publicFunc();

    virtual void publicFunc2() {
        // nothing really, just for mocks to catch
    };

signals:
    void otherServiceSignal(QString data);

public slots:
    void otherServiceSlot(QString data);
};

#endif // OTHERSERVICE_H
