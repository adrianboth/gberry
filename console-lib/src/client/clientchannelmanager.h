#ifndef CLIENTCHANNELMANAGER_H
#define CLIENTCHANNELMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QScopedPointer>

#include "clientsidechannelpartners.h"
#include "playermeta.h"

class ClientSidePlayerChannel;
class ClientSideControlChannel;
class ClientChannelManagerPrivate;
class ClientSideChannelPartnerImpl;


class ClientChannelManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientChannelManager(QObject *parent = 0);
    ~ClientChannelManager();

    void registerControlChannel(ClientSideControlChannel* channel);

    QList<int> allPlayerChannelIds();
    void closePlayerChannel(int channelId);
    void closeAllPlayerChannels();

signals:
    // send messages towards server
    void outgoingMessage(int channelId, const QByteArray msg);

    void newPlayerChannel(ClientSidePlayerChannel* channel, PlayerMeta playerMeta);

public slots:
    virtual void processMessage(int channelId, const QByteArray& msg);

private:
    friend class ClientSideChannelPartnerImpl;
    const QScopedPointer<ClientChannelManagerPrivate> _d;
};
#endif // CLIENTCHANNELMANAGER_H
