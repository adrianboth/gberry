#include "clientsidechannelmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "client/clientsideplayerchannel.h"


ClientSideChannelManager::ClientSideChannelManager(QObject* parent) :
    ChannelManager(parent)
{
}

ClientSideChannelManager::~ClientSideChannelManager()
{
}

// overrides function in ChannelManager
void ClientSideChannelManager::processMessage(int channelId, const QByteArray msg)
{
    qDebug() << "### ClientSideChannelManager::processMessage: cid=" << channelId;

    if (!_channels.contains(channelId))
    {
        QJsonDocument doc(QJsonDocument::fromJson(msg));
        QJsonObject json(doc.object());
        // TODO: common impl for json validation
        if (json.contains("command") && json["command"] == "OpenChannel")
        {
            qDebug("### ClientSideChannelManager OpenChannel");
            // TODO: handle metadata
            if (!json.contains("player_metadata"))
                return;

            // there is PlayerMetadata -> this is Player channel
            PlayerMetadata playerMeta(json["player_metadata"].toObject());

            // note controls created by ChannelManager will be deleted when
            // ChannelManager is destroyed (if not specifically before that)
            ClientSidePlayerChannel* playerControl =
                    new ClientSidePlayerChannel(channelId, playerMeta);
            registerChannel(playerControl);

            playerControl->setState(Channel::CHANNEL_OPEN);
            emit newPlayerChannel(playerControl, playerMeta);
            emit ChannelManager::newChannel(channelId);

            QJsonObject json;
            json["command"] = "OpenChannelAccepted"; // TODO: how denial would go, we want need to make query instead of signal (above)
            QJsonDocument jsonDoc(json);

            QByteArray msg = jsonDoc.toJson();

            emit outgoingMessage(channelId, msg);
        }
        else
        {
            qWarning() << "Message for unhandled channel, id =" << channelId;
        }
        qDebug("### OUT2 ClientSideChannelManager::processMessage()");
        return;
    }

    ChannelManager::processMessage(channelId, msg);
    qDebug("### OUT ClientSideChannelManager::processMessage()");
}

void ClientSideChannelManager::channelCloseReceived(int channelId)
{
    // on client side we delete channels
    Channel* handler = unregisterChannel(channelId);
    if (handler)
        handler->deleteLater();

    ChannelManager::channelCloseReceived(channelId);
}
