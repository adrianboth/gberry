#ifndef CLIENTSIDECONTROLCHANNEL_H
#define CLIENTSIDECONTROLCHANNEL_H

#include <QObject>

class QByteArray;
class ClientSideChannelPartner;

class ClientSideControlChannel : public QObject
{
    Q_OBJECT

public:
    ClientSideControlChannel();
    ~ClientSideControlChannel();

    void requestApplicationLaunch(QString appID);
    void requestApplicationExit();

    static const int CHANNEL_ID;

    void setApplicationIdCode(const QString& code);

    void ping();

    virtual bool receiveMessage(const QByteArray& msg);

    void attachChannelPartner(ClientSideChannelPartner* partner);
    void detachChannelPartner();

    int channelId();

signals:
    void channelClosed();
    void pingReceived();

private:
    int _id;
    QString _applicationIdCode;
    ClientSideChannelPartner* _partner;

};

#endif // CLIENTSIDECONTROLCHANNEL_H
