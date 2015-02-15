#include "gtest/gtest.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // TODO: what about event loop

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
