#include <QCoreApplication>
#include <QDebug>

#include "classundertest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "STARTED APP";
    ClassUnderTest cut;
    QString ret = cut.publicFunc();
    qDebug() << "Return value of publicFunc():" << ret;

    cut.triggerSignal1();
    cut.slot1("Calling slot1 directly from main()");

    // goes to event loop, doesn't exit automatically

    // TODO: impl exit through signal (event handling, e.q oneshot timer)
    return a.exec();
}
