#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <QObject>
#include <QMap>

#include "channelhandler.h"


class ChannelManager : public QObject
{
    Q_OBJECT
public:
    explicit ChannelManager(QObject *parent = 0);
    ~ChannelManager();

    const static int ROOT_CHANNEL = 0;

    void registerHandler(ChannelHandler* handler);
    ChannelHandler* unregisterHandler(int channelId);


    // TODO: how channel is opened towards commXX
    // TODO: how channel is closed
    //   - little bit different handling on comms and app

    //  because there is no socket connection for channels
    //    - pings required to verify channel is open
    //    - or explicit control message (mobile device away)
    //        -> message to channel


signals:
    void outgoingMessage(int channelId, const QByteArray& msg);
    void newChannel(int channelId);
    void channelClosed(int channelId);

public slots:
    void processMessage(int channelId, const QByteArray& msg);
    void outgoingMessageFromChannelHandler(const QByteArray& msg);

protected:
    QMap<int, ChannelHandler*> _handlers; // by channelId


};

#endif // CHANNELMANAGER_H
