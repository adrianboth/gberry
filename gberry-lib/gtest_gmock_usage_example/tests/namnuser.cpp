#include "namnuser.h"

NAMNUser::NAMNUser(QNetworkAccessManager* qnam, QObject *parent) : QObject(parent), httpFinishedCalled(false)
{
    if (qnam)
        _qnam = qnam;
    else
        _qnam = new QNetworkAccessManager(this);

}

NAMNUser::~NAMNUser()
{

}

void NAMNUser::makeQuery(QUrl url)
{
    _reply = _qnam->get(QNetworkRequest(url));

    connect(_reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
}

void NAMNUser::httpFinished()
{
     httpFinishedCalled = true;
}
