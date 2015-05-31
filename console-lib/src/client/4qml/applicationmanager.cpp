#include "applicationmanager.h"

ApplicationManager::ApplicationManager(ClientSideControlChannel* controlChannel, QObject *parent) :
    QObject(parent),
    _controlChannel(controlChannel)
{
    Q_ASSERT(controlChannel != NULL);
}

ApplicationManager::~ApplicationManager()
{

}

void ApplicationManager::launchApplication(QString appID)
{
    _controlChannel->requestApplicationLaunch(appID);
}

void ApplicationManager::exitApplication()
{
    _controlChannel->requestApplicationExit();
}
