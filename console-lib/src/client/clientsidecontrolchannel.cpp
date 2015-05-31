#include "clientsidecontrolchannel.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>


ClientSideControlChannel::ClientSideControlChannel()
{
}

ClientSideControlChannel::~ClientSideControlChannel()
{
    qDebug("### ~ClientSideControlChannel");
}

void ClientSideControlChannel::requestApplicationLaunch(QString appID)
{
    QJsonObject json;
    json["command"] = "LaunchApplication";
    json["app_id"] = appID;
    QJsonDocument jsonDoc(json);
    QByteArray msg = jsonDoc.toJson();
    sendMessage(msg);
}

void ClientSideControlChannel::requestApplicationExit()
{
    QJsonObject json;
    json["command"] = "ExitApplication";
    QJsonDocument jsonDoc(json);
    QByteArray msg = jsonDoc.toJson();
    sendMessage(msg);
}
