#ifndef GTESTRUNNER_H
#define GTESTRUNNER_H

#include <QObject>

class GTestRunner : public QObject
{
    Q_OBJECT
public:
    explicit GTestRunner(int* argc, char** argv, QObject *parent = 0);
    ~GTestRunner();

signals:

public slots:
    void runTests();

};

#endif // GTESTRUNNER_H
