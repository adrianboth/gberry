#include "gtestrunner.h"

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTimer>


GTestRunner::GTestRunner(int* argc, char** argv, QObject *parent) : QObject(parent)
{
    ::testing::InitGoogleTest(argc, argv);
}

GTestRunner::~GTestRunner()
{

}

void GTestRunner::start()
{
    QTimer::singleShot(0, this, SLOT(runTests()));
}

void GTestRunner::runTests()
{
    int retval = RUN_ALL_TESTS();
    QCoreApplication::exit(retval);
}
