#ifndef CLIENTSIDECONTROLCHANNEL_H
#define CLIENTSIDECONTROLCHANNEL_H

#include <QObject>
#include <QScopedArrayPointer>

#include "server/icommand.h"

class QByteArray;
class ClientSideChannelPartner;
class ClientSideControlChannelPrivate;

class ClientSideControlChannel : public QObject
{
    Q_OBJECT

public:
    ClientSideControlChannel();
    ~ClientSideControlChannel();

    void requestApplicationLaunch(QString appID);
    void requestApplicationExit();

    static const int CHANNEL_ID;

    void setApplicationCode(const QString& code);

    void ping();

    bool receiveMessage(const QByteArray& msg);
    void sendMessage(const QByteArray& msg);


    void attachChannelPartner(ClientSideChannelPartner* partner);
    void detachChannelPartner();

    int channelId() const;

    bool isConnected() const;
    bool isActivated() const;

    // takes ownership of command
    void registerCommand(ICommand* cmd);

signals:
    void channelClosed();
    void pingReceived();

    void isConnectedChanged();
    void isActivatedChanged();

public slots:
    void connectionBroken();

private:
    const QScopedPointer<ClientSideControlChannelPrivate> _d;

};

#endif // CLIENTSIDECONTROLCHANNEL_H
