#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "ichannelparent.h"
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
    enum ChannelState {
        CHANNEL_CLOSED,
        CHANNEL_OPEN_ONGOING,
        CHANNEL_OPEN,
        // TODO: currently seems that restricted open doesn't have meaning, because apps handle that ... but is there good reason to handle that general way?
        CHANNEL_RESTRICTED_OPEN // TODO: e.g. joining to game denied (might be just temporary, game ongoing), but restricted channel open to receive information from app (e.g. when joining possible)

    };

    Channel(int channelId, IChannelParent* channelParent = 0, QObject* qParent = 0);
    virtual ~Channel();
    void setChannelHandlerParent(IChannelParent* channelParent);
    void detachChannelHandlerParent();

    int channelId();

    virtual ChannelState state();
    virtual void setState(ChannelState newState);
    virtual void reopen() = 0;

    Q_INVOKABLE virtual bool isOpen();
    virtual void close();

    virtual bool processJsonMessage(const QJsonObject& json);

public slots:
    // true if message was handled, used in subclasses to know if parent handled to message
    virtual bool receiveMessage(const QByteArray msg);

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
