#include "restquery.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

RESTQuery::RESTQuery() : QObject(NULL), qnam(this), reply(NULL)
{

}

RESTQuery::~RESTQuery()
{

}


void RESTQuery::startQuery(QUrl url)
{
    qDebug() << "GET:" << url;
    // TODO: could we pass reply with signal
    // TODO: is there signal for error situation
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            this,  &RESTQuery::httpFinished);

}

void RESTQuery::httpFinished()
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        qDebug() << "HTTP ERROR:" << reply->errorString();
        reply->deleteLater();

        emit queryFinished(""); // TODO: dummy way to tell error occurred
        return;

    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "Handled redirect to:" << newUrl;
        reply->deleteLater();
        startQuery(newUrl);
        return;

    } else {
        // we have data
        QString data = reply->readAll();
        qDebug() << "DATA:" << data;
        emit queryFinished(data);
    }

    reply->deleteLater();
    reply = 0;

}
