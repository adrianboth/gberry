#include "classundertest.h"

#include <QDebug>

#include "otherservice.h"


ClassUnderTest::ClassUnderTest(IOtherService *other)
{
    if (other)
    {
        // especially for testing
        _other = other;
    }
    else
    {
        // default behaviour
        _other = new OtherService();
    }
}

ClassUnderTest::~ClassUnderTest()
{
    //delete _other;
}

QString ClassUnderTest::publicFunc()
{
    qDebug() << "Handling ClassUnderTest::publicFunc()";
    return _other->publicFunc();
}

void ClassUnderTest::triggerSignal1()
{
    emit signal1("Trigger signal1");
}

void ClassUnderTest::slot1(QString data)
{
    qDebug() << "slot1:" << data;
    _other->publicFunc2();
}
