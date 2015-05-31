#include "localapplicationsstorage.h"

LocalApplicationsStorage::LocalApplicationsStorage(QObject *parent) : QObject(parent)
{

}

LocalApplicationsStorage::~LocalApplicationsStorage()
{

}

QList<QPointer<ApplicationMeta>> LocalApplicationsStorage::applications()
{

    // TODO: just temporary
    QList<QPointer<ApplicationMeta>> apps;
    QString waitAppPath("/home/tevuore/workspace/gberry/builds/build-waitapp-Desktop_Qt_5_4_0_GCC_64bit-Debug/waitapp");
    QString mainuiAppPath("/home/tevuore/workspace/gberry/builds/build-mainui-Desktop_Qt_5_4_0_GCC_64bit-Debug/mainui");
    QString tictactoeAppPath("/home/tevuore/workspace/gberry/builds/build-tictactoe-Desktop_Qt_5_4_0_GCC_64bit-Debug/tictactoe");

    QPointer<ApplicationMeta> waitApp = new ApplicationMeta("waitapp", waitAppPath);
    apps.append(waitApp);

    QPointer<ApplicationMeta> mainui = new ApplicationMeta("mainui", mainuiAppPath);
    apps.append(mainui);

    QPointer<ApplicationMeta> tictactoe = new ApplicationMeta("tictactoe", tictactoeAppPath);
    apps.append(tictactoe);

    return apps;
}
