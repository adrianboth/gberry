#ifndef RESTQUERY_H
#define RESTQUERY_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>

class RESTQuery : public QObject
{
    Q_OBJECT

public:
    RESTQuery();
    virtual ~RESTQuery();

    void startQuery(QUrl url);

signals:
    void queryFinished(QString data);

private slots:
    void httpFinished();

private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QUrl url; // TODO: hide this
};

#endif // RESTQUERY_H
