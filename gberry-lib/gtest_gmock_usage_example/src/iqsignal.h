#ifndef IQSIGNAL_H
#define IQSIGNAL_H

#include <QObject>
#include <QString>
#include <QtPlugin>

// testing inheritance
class IQSignal : public QObject
{
    Q_OBJECT

signals:
    void signal2(QString data);

public slots:
    // note: typically slot are not virtual, but if not declared so
    //       signal handling ends up here in default impl
    virtual void slot1(QString data) = 0;

};

Q_DECLARE_INTERFACE(IQSignal, "gberry.qsignal/1.0")


#endif // IQSIGNAL_H

