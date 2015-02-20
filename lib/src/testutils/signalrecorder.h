#ifndef SIGNALRECORDER_H
#define SIGNALRECORDER_H

#include <QObject>

class SignalRecorder : public QObject
{
    Q_OBJECT
public:
    explicit SignalRecorder(QObject *parent = 0) : QObject(parent), _received(false) {}
    ~SignalRecorder() {}

    bool received() { return _received; }
    bool received_QObjectPointer() { return _received && _qobj != NULL; }
    QObject* getQObjectPointer() { return _qobj; }
    void reset() { _received = false; _qobj=nullptr; }

signals:

public slots:
    void signal0() { _received = true; }
    void signal1_QObjectPointer(QObject* qobj) { _qobj = qobj; _received = true; }

private:
    bool _received;
    QObject* _qobj;

};

#endif // SIGNALRECORDER_H
