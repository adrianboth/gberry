#ifndef CHANNELHANDLER_H
#define CHANNELHANDLER_H

#include <QObject>

#include "lib_global.h"


class LIBSHARED_EXPORT ChannelHandler : public QObject
{
    Q_OBJECT

public:
    enum ChannelState {
        CHANNEL_CLOSED,
        CHANNEL_OPEN_ONGOING,
        CHANNEL_OPEN,
        CHANNEL_RESTRICTED_OPEN // TODO: e.g. joining to game denied (might be just temporar, game ongoing), but restricted channel open to receive information from app (e.g. when joining possible)

    };

    ChannelHandler(int channelId, QObject* parent = 0);

    int channelId();

    virtual ChannelState state();
    virtual void setState(ChannelState newState);


signals:
    void outgoingMessage(const QByteArray& msg);

public slots:
    virtual void handleMessage(const QByteArray& msg) = 0;

private:
    int _id;
    ChannelState _state;
};

#endif // CHANNELHANDLER_H
