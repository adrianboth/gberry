#ifndef CLASSUNDERTEST_H
#define CLASSUNDERTEST_H

#include <QObject>

#include <iqsignal.h>


class IOtherService;

class ClassUnderTest : public IQSignal
{
    Q_OBJECT
    Q_INTERFACES(IQSignal)

public:
    ClassUnderTest(IOtherService* other = Q_NULLPTR);
    virtual ~ClassUnderTest();

    void triggerSignal1();

    QString publicFunc();

signals:
    void signal1(QString data);
    void signal2(QString data);

public slots:
    void slot1(QString data);

private:
    IOtherService* _other;
};

#endif // CLASSUNDERTEST_H
