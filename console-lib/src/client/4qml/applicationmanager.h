#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QObject>

#include "client/clientsidecontrolchannel.h"


class ApplicationManager : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationManager(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    ~ApplicationManager();

    Q_INVOKABLE void launchApplication(QString appID);
    Q_INVOKABLE void exitApplication();

signals:

public slots:

private:
    ClientSideControlChannel* _controlChannel;
};

#endif // APPLICATIONMANAGER_H
