#ifndef NAMNUSER_H
#define NAMNUSER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

// TODO: trying to solve mocking
//template <class NetworkAccessManager>
class NAMNUser : public QObject
{
    Q_OBJECT
public:
    // TODO: replaced
    explicit NAMNUser(QNetworkAccessManager* qnam = 0, QObject *parent = 0);
    ~NAMNUser();

    void makeQuery(QUrl url);

    bool httpFinishedCalled;

signals:

public slots:
    void httpFinished();

private:
    QNetworkReply* _reply;
    QNetworkAccessManager* _qnam;
    //NetworkAccessManager* _qnam;
};

#endif // NAMNUSER_H
