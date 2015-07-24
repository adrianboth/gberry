#include "otherservice.h"

#include <QDebug>

OtherService::OtherService(QObject *parent) : QObject(parent)
{

}

OtherService::~OtherService()
{

}

void OtherService::triggerOtherServiceSignal()
{
    emit otherServiceSignal("Triggered otherServiceSignal()");
}

QString OtherService::publicFunc()
{
    qDebug() << "OtherService::Called publicFunc()";
    return "publicFuncReturn";
}

void OtherService::otherServiceSlot(QString data)
{
    qDebug() << "Handling otherServiceSlot():" << data;
}
