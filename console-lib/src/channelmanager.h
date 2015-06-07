#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>

#include "ichannelparent.h"
#include "icommand.h"
#include "channel.h"


class ChannelManager : public QObject, IChannelParent
{
    Q_OBJECT
public:
    explicit ChannelManager(QObject *parent = 0);
    ~ChannelManager();

    const static int ROOT_CHANNEL = 0;

    void registerChannel(Channel* handler);

    // If channel is open then it is closed before unattaching ChannelHandler
    // from channel.
    Channel* unregisterChannel(int channelId);

    // Closes channel and sends closing message. ChannelHandler is left
    // attached to the channel.
    void closeChannel(int channelId);

    QList<int> allChannelIds();

    void registerCommand(QSharedPointer<ICommand> cmd);

    // TODO: how channel is opened towards commXX
    // TODO: how channel is closed
    //   - little bit different handling on comms and app

    //  because there is no socket connection for channels
    //    - pings required to verify channel is open
    //    - or explicit control message (mobile device away)
    //        -> message to channel

    // from IChannelParent
    virtual void channelSendMessage(int channelId, const QByteArray msg) override;
    virtual void channelCloseReceived(int channelId) override;
    virtual void channelDestroyed(int channelId) override;
    virtual bool processCommand(int channelId, const QJsonObject &json) override;

signals:
    void outgoingMessage(int channelId, const QByteArray msg);
    void newChannel(int channelId);
    void channelClosed(int channelId);

public slots:
    virtual void processMessage(int channelId, const QByteArray msg);

protected:
    QMap<int, Channel*> _channels; // by channelId
    QMap<QString, QSharedPointer<ICommand>> _commands;


};

#endif // CHANNELMANAGER_H
