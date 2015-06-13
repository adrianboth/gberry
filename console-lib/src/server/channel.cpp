#include "channel.h"

#include <QJsonDocument>
#include <QJsonObject>

#define LOG_AREA "Channel"
#include "log/log.h"

Channel::Channel(
        int channelId,
        ChannelSouthPartner* channelParent,
        QObject* parent) :
    QObject(parent),
    _id(channelId),
    _isOpen(false),
    _southPartner(channelParent)
{
}

Channel::~Channel()
{
    TRACE("~Channel");
    foreach (auto cmd, _commands)
        delete cmd;
}

void Channel::attachSouthPartner(ChannelSouthPartner* partner)
{
    detachSouthPartner();
    _southPartner = partner;
}

void Channel::detachSouthPartner()
{
    if (_southPartner) delete _southPartner;
    _southPartner = nullptr;
}

int Channel::channelId() const
{
    return _id;
}

void Channel::sendMessageToSouth(const QByteArray& msg)
{
    if (_southPartner && isOpen())
    {
        _southPartner->channelSendMessageToSouth(msg);
    }
}

void Channel::processCloseReceived()
{
    _isOpen = false;
    if (_southPartner)
    {
        _southPartner->channelCloseReceived();
    }
    emit channelClosed();
}

bool Channel::isOpen() const
{
    return _isOpen;
}

void Channel::open()
{
    _isOpen = true;
}

void Channel::close()
{
    _isOpen = false;
    emit channelClosed();
}

bool Channel::receiveMessageFromSouth(const QByteArray& msg)
{
    QJsonParseError errors;

    QJsonDocument doc(QJsonDocument::fromJson(msg, &errors));
    if (errors.error != QJsonParseError::NoError) {
        WARN("Failed to parse json message: error =" << errors.errorString() << ", offset =" << errors.offset << ", message =" << msg);
        return false;
    }

    QJsonObject json(doc.object());
    return processJsonMessage(json); // virtual
}


class CommandResponseImpl : public ICommandResponse
{
public:
    virtual void set(QJsonObject j) override { json = j; }
    QJsonObject json;
};


bool Channel::processJsonMessage(const QJsonObject &json)
{
    if (json.contains("command") && _commands.contains(json["command"].toString())) {
        CommandResponseImpl response;
        // commands are not required to have response message
        // 'result' tells if msg was processed
        bool result = _commands[json["command"].toString()]->process(json, response);
        if (result && !response.json.empty()) {
            // TODO: should we automatically add "command_response" ?
            QJsonDocument jsonDoc(response.json);
            _southPartner->channelSendMessageToSouth(jsonDoc.toJson());
        }

        return result;
    }
    // msg didn't match
    return false;
}

void Channel::registerCommand(ICommand *cmd)
{
    if (cmd != nullptr) {
        _commands[cmd->id()] = cmd;
    }
}
