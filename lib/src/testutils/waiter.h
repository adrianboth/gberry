#ifndef WAITER_H
#define WAITER_H

#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <functional>

#include <gtest/gtest.h>

// for understanding lambdas check
// http://www.cprogramming.com/c++11/c++11-lambda-closures.html

static const int DEFAULT_TIMEOUT_MS = 100;
static const int DEFAULT_WAITING_STEP_MS = 10;


class Waiter
{
//
public:
    Waiter(std::function<bool ()> func,
           int timeout_ms=DEFAULT_TIMEOUT_MS,
           int waitingStep_ms = DEFAULT_WAITING_STEP_MS) :
        _maxTime(timeout_ms), _waitingStep(waitingStep_ms), _loopCounter(0), _startTime(NULL), _func(func)
    {
    }

    ~Waiter()
    {
        if (_startTime)
            delete _startTime;
    }

    void wait(bool debug = false)
    {
        _startTime = new QTime();
        _startTime->start();

        if (debug)
            qDebug() << "Starting waiting: condition =" << _func();

        while (_startTime->elapsed() < _maxTime && !_func())
        {
            QCoreApplication::processEvents();
            QThread::msleep(_waitingStep);
            _loopCounter++;
        }
        if (debug)
            qDebug() << "Waited" << _loopCounter << "loops, condition =" << _func();

        // TODO: record reason of getting out of loop
        // TODO: util to evaluate func() (for asserts)
        // TODO: debug printing what happens
    }

    void resetAndWait()
    {
        _loopCounter = 0;
        if (_startTime)
        {
            delete _startTime;
            _startTime = NULL;
        }
        wait();
    }

    static void wait(std::function<bool ()> func,
                     bool debug=false,
                     int timeout_ms=DEFAULT_TIMEOUT_MS,
                     int waitingStep_ms=DEFAULT_WAITING_STEP_MS)
    {
        Waiter waiter(func, timeout_ms, waitingStep_ms);
        waiter.wait(debug);
    }

    // waits conditions and finally assert it is true
    static void waitAndAssert(std::function<bool ()> func,
                     bool debug=false,
                     int timeout_ms=DEFAULT_TIMEOUT_MS,
                     int waitingStep_ms=DEFAULT_WAITING_STEP_MS)
    {
        Waiter waiter(func, timeout_ms, waitingStep_ms);
        waiter.wait(debug);
        ASSERT_TRUE(func());
    }

private:
    int _maxTime;
    int _waitingStep;
    int _loopCounter; // debug information
    QTime* _startTime;
    std::function<bool ()> _func;

};

#endif // WAITER_H
