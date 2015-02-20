//#include "gtest/gtest.h"

#include <QCoreApplication>
#include <QTimer>

#include "gtestrunner.h"

#include "gberryconsoleserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // TODO: what about event loop
    //GBerryConsoleServer gbserver;

    GTestRunner runner(&argc, argv);
    // TODO: put this to runner
    QTimer::singleShot(0, &runner, SLOT(runTests()));

    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return a.exec();
}
