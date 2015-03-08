#include "logcontrol.h"

#include <QList>
#include <QDebug>

#include "ilogmsghandler.h"

class LogControl::PrivateLogControlImpl
{
public:
    QList<ILogMsgHandler*> handlers;
};

LogControl::LogControl() : _impl(nullptr), _highestLogLevel(Log::NONE)
{
    _impl = new PrivateLogControlImpl();
}

LogControl::~LogControl()
{
    delete _impl;
    // TODO: delete also handlers, document
}

void LogControl::registerMsgHandler(ILogMsgHandler* msgHandler)
{
    _impl->handlers.append(msgHandler);
    int logLevel = msgHandler->logLevel();
    if (logLevel > _highestLogLevel)
    {
        _highestLogLevel = logLevel;
        emit highestLogLevel();
    }
}

QList<ILogMsgHandler*> LogControl::handlers(const QString &msg) const
{
    Q_UNUSED(msg)
    // TODO: impl handler filtering, now return all
    return _impl->handlers;
}
