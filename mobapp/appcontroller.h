#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

#include "restquery.h"

class QTimer;


class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint serverPingOKCounter READ serverPingOKCounter NOTIFY serverCounterChanged)
    Q_PROPERTY(uint serverPingFailureCounter READ serverPingFailureCounter NOTIFY serverCounterChanged)
    Q_PROPERTY(bool serverConnectionOK READ serverConnectionOK NOTIFY serverStatusChanged)

    Q_PROPERTY(uint consolePingOKCounter READ consolePingOKCounter NOTIFY consoleCounterChanged)
    Q_PROPERTY(uint consolePingFailureCounter READ consolePingFailureCounter NOTIFY consoleCounterChanged)
    Q_PROPERTY(bool consoleConnectionOK READ consoleConnectionOK NOTIFY consoleStatusChanged)

public:
    AppController();
    ~AppController();

    // propery functions
    uint serverPingOKCounter() const { return _serverPingOKCounter; }
    uint serverPingFailureCounter() const { return _serverPingFailureCounter; }
    bool serverConnectionOK() const { return _serverConnectionOK; }
    uint consolePingOKCounter() const { return _consolePingOKCounter; }
    uint consolePingFailureCounter() const { return _consolePingFailureCounter; }
    bool consoleConnectionOK() const { return _consoleConnectionOK; }

signals:
    void serverCounterChanged();
    void consoleCounterChanged();
    void serverStatusChanged();
    void consoleStatusChanged();

private slots:
    void timerUpdate();
    void serverQueryReady(QString data); // TODO: can there be parallel requests?
    void consoleQueryReady(QString data);

private:
    bool _serverConnectionOK;
    bool _serverPingReceived;
    uint _serverPingOKCounter;
    uint _serverPingFailureCounter;

    bool _consoleConnectionOK;
    bool _consolePingReceived;
    uint _consolePingOKCounter;
    uint _consolePingFailureCounter;

    RESTQuery _serverQuery;
    RESTQuery _consoleQuery;

    QTimer* _timer;
};

#endif // APPCONTROLLER_H
