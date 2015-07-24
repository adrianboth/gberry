#ifndef SIGNALUTIL_H
#define SIGNALUTIL_H

#include <QObject>

#include "classundertest.h"
#include "iqsignal.h"

// TODO: maybe we could have a template or macro to create this kinds of SignalUtil classes

class SignalUtil : public QObject
{
    Q_OBJECT

public:
    void connectSignals(ClassUnderTest* cut, IQSignal* iqs)
    {
        connect(cut, &ClassUnderTest::signal1,
                iqs, &IQSignal::slot1);
    }

    void connectSignals2(IQSignal* iqs, ClassUnderTest* cut)
    {
        connect(iqs, &IQSignal::signal2,
                cut, &ClassUnderTest::slot1);
    }
};

#endif // SIGNALUTIL_H

