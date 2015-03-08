#ifndef LOGCONTROL_H
#define LOGCONTROL_H

#include <QObject>
#include <QList>

// forward declarations
class ILogMsgHandler;

class LogControl : public QObject
{
    Q_OBJECT

public:
    LogControl();
    ~LogControl();
    // TODO: copy constructors and such

    void registerMsgHandler(ILogMsgHandler* msgHandler);

    QList<ILogMsgHandler*> handlers(const QString& logArea) const;

    int highestLogLevel() const { return _highestLogLevel; }

signals:
    void highestLogLevelChanged();

private:
    class PrivateLogControlImpl;
    PrivateLogControlImpl* _impl;
    int _highestLogLevel;
};

#endif // LOGCONTROL_H
