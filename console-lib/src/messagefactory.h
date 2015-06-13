#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include <QByteArray>
#include <QJsonObject>

#include "playermeta.h"

namespace GBerry {
namespace Console {

class MessageFactory
{
public:

    static QByteArray createPingCommand();
    static QByteArray createPingReply();
    static QByteArray createPingCommand(const QString& applicationIdCode);
    static QByteArray createPingReply(const QString& applicationIdCode);
    static QJsonObject createPingCommandJson();
    static QJsonObject createPingReplyJson();

    static QByteArray createOpenChannelCommand(PlayerMeta PlayerMeta);
    static QByteArray createOpenChannelReply();
    static QByteArray createCloseChannelCommand();

    static QByteArray createPlayerMessage(QJsonObject playerData);
    static QByteArray createPlayerMessage(QString str);

private:
    MessageFactory();
    ~MessageFactory();
};

}} // eon

#endif // MESSAGEFACTORY_H
