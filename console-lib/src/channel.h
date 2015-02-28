#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "ichannelparent.h"
#include "lib_global.h"


class LIBSHARED_EXPORT Channel : public QObject
{
    Q_OBJECT

public:
    enum ChannelState {
        CHANNEL_CLOSED,
        CHANNEL_OPEN_ONGOING,
        CHANNEL_OPEN,
        CHANNEL_RESTRICTED_OPEN // TODO: e.g. joining to game denied (might be just temporary, game ongoing), but restricted channel open to receive information from app (e.g. when joining possible)

    };

    Channel(int channelId, IChannelParent* channelParent = 0, QObject* qParent = 0);
    virtual ~Channel();
    void setChannelHandlerParent(IChannelParent* channelParent);

    int channelId();

    virtual ChannelState state();
    virtual void setState(ChannelState newState);
    virtual void reopen() = 0;

    virtual bool isOpen();
    virtual void close();

public slots:
    virtual void receiveMessage(const QByteArray msg) = 0;

signals:
    void channelClosed();

protected:
    int _id;
    ChannelState _state;

    void sendMessage(const QByteArray msg);
    void closeReceived();

private:
    IChannelParent* _channelParent;

};

#endif // CHANNEL_H
