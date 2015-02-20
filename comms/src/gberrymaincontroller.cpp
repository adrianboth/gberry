#include "gberrymaincontroller.h"

#include <QtNetwork>
#include <QDebug>

// TODO: new logging API
// TODO: c++11 features for signal connection


GBerryMainController::GBerryMainController(QNetworkAccessManager* network) : qnam(network)
{
//    connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
//            this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
//#ifndef QT_NO_SSL
//    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
//            this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
//#endif

}

GBerryMainController::~GBerryMainController()
{

}

void GBerryMainController::start()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

void GBerryMainController::startRequest(QUrl url)
{
    qDebug() << "GET:" << url;
    reply = qnam->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            this,  &GBerryMainController::httpFinished);

    //connect(reply, SIGNAL(finished()),
    //        this, SLOT(httpFinished()));

    // TODO: not needed in our example (proxy doesn't yet support this)
    //connect(reply, SIGNAL(readyRead()),
    //        this, SLOT(httpReadyRead()));
}

// TODO: maybe something needs to be done to common lib

void GBerryMainController::timerUpdate()
{
    // TODO: we would need to check if previous http call has finished
    // TODO: reading configuration from somewhere
    startRequest(QUrl("http://localhost:9000/gberryrest/v1/ping/"));
}

void GBerryMainController::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply

    // TODO: for simple REST call no reason to read partial data ...
    //if (file)
    //    file->write(reply->readAll());
}


void GBerryMainController::httpFinished()
{

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        qDebug() << "HTTP ERROR:" << reply->errorString();
        reply->deleteLater();
        return;

    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "Handled redirect to:" << newUrl;
        reply->deleteLater();
        startRequest(newUrl);
        return;

    } else {
        // we have data
        qDebug() << "DATA:" << reply->readAll();
    }

    reply->deleteLater();
    reply = 0;
}
