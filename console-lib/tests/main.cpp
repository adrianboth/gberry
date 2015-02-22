#include <QCoreApplication>

#include <testutils/gtestrunner.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GTestRunner runner(&argc, argv);
    runner.start();
    return a.exec();
}
