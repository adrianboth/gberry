#ifndef CONTROLCHANNEL_H
#define CONTROLCHANNEL_H

#include <QObject>
#include <QByteArray>

#include "channel.h"


class ControlChannel : public Channel
{
    Q_OBJECT

public:
    ControlChannel();
    ~ControlChannel();

    void ping();

    // Channel
    virtual void reopen() {} // // do nothing
    virtual void receiveMessage(const QByteArray msg);

signals:
    void pingReceived();
};

#endif // CONTROLCHANNEL_H
