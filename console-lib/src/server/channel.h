#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QMap>

#include "channelsouthpartner.h"
#include "icommand.h"
#include "lib_global.h"

/**
 * @brief The Channel class
 *
 * Message Flow
 * ------------
 *
 * Channel receives messages only specific for the channel:
 *
 *  - receiveMessage(..) is called when new message arrives
 *  - by default receiveMessage() tries to parse message as JSON
 *  - processJsonMessage(..) is called with successfully parsed JSON
 *  - by default processJsonMessage() delegates handling for channel parent via
 *    IChannelParent::processCommand(..)
 *  - ChannelManager (who is IChannelParent) checks if json msg has 'command'
 *     ember and if the member matches to any registered commands
 *      -> ICommand::process(..) is invoked.
 *
 * Subclasses of Channel class can override both receiveMessage(..) and
 * processCommand(..) to implement own handling.
 *
 */
class LIBSHARED_EXPORT Channel : public QObject
{
    Q_OBJECT

public:
    Channel(int channelId, ChannelSouthPartner* partner = 0, QObject* qParent = 0);
    virtual ~Channel();
    void attachSouthPartner(ChannelSouthPartner* partner);
    void detachSouthPartner();

    int channelId() const;

    virtual bool isOpen() const;
    virtual void open();
    // closing is more like pausing -> TODO: maybe better naming, TODO: playerchannels close, controlchannels are destroyed after closing
    virtual void close();

    // takes ownership of command
    void registerCommand(ICommand* cmd);

public slots:
    // true if message was handled, used in subclasses to know if parent handled to message
    virtual bool receiveMessageFromSouth(const QByteArray& msg);

signals:
    void channelClosed();

protected:
    int _id;
    bool _isOpen;
    ChannelSouthPartner* _southPartner;
    QMap<QString, ICommand*> _commands;

    virtual bool processJsonMessage(const QJsonObject& json);
    void processCloseReceived();
    void sendMessageToSouth(const QByteArray& msg);

};

#endif // CHANNEL_H
