#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>

#include "lib_global.h"

class ChannelHandler
{
public:
    int channelId();
    void handle(QString data); // TODO: should be signal?

    // TODO: writing also here?
};

class LIBSHARED_EXPORT MessageHandler : public QObject
{
    Q_OBJECT

public:
    MessageHandler();

public slots:
    void handleData(QByteArray& data);

};

#endif // MESSAGEHANDLER_H
