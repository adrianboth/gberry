#include "gtestrunner.h"

#include <gtest/gtest.h>
#include <QCoreApplication>


GTestRunner::GTestRunner(int* argc, char** argv, QObject *parent) : QObject(parent)
{
    ::testing::InitGoogleTest(argc, argv);
}

GTestRunner::~GTestRunner()
{

}

void GTestRunner::runTests()
{
    int retval = RUN_ALL_TESTS();
    QCoreApplication::exit(retval);
    // TODO: exit event loop
}
