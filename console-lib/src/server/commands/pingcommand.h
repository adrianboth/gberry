#ifndef PINGCOMMAND_H
#define PINGCOMMAND_H

#include "server/icommand.h"

class ServerSideControlChannel;

namespace GBerry {
namespace Console {
namespace Server {

class PingCommand : public ICommand
{
public:
    explicit PingCommand(ServerSideControlChannel* manager);
    virtual ~PingCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

private:
    ServerSideControlChannel* _manager;

};

class PingReply : public ICommand
{
public:
    explicit PingReply(ServerSideControlChannel* manager);
    virtual ~PingReply();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

private:
    ServerSideControlChannel* _manager;

};

}}} // eon

#endif // PINGCOMMAND_H
