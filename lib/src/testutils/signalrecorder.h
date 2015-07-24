/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #ifndef SIGNALRECORDER_H
#define SIGNALRECORDER_H

#include <QObject>

class SignalRecorder : public QObject
{
    Q_OBJECT
public:
    explicit SignalRecorder(QObject *parent = 0) : QObject(parent), _received(false), _int(-1) {}
    ~SignalRecorder() {}

    bool received() { return _received; }
    bool received_QObjectPointer() { return _received && _qobj != NULL; }
    bool received_int() { return _received && _int != -1; } // TODO: this just fast, -1 should allowed value

    QObject* getQObjectPointer() { return _qobj; }
    int getInt() { return _int; }

    void reset() { _received = false; _qobj=nullptr; }

signals:

public slots:
    void signal0() { _received = true; }
    void signal1_QObjectPointer(QObject* qobj) { _qobj = qobj; _received = true; }
    void signal1_int(int i) { _int = i; _received = true; }

private:
    bool _received;
    QObject* _qobj;
    int _int;
};

#endif // SIGNALRECORDER_H
