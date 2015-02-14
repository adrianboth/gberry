#ifndef GBERRYMAINCONTROLLER_H
#define GBERRYMAINCONTROLLER_H

#include <QNetworkAccessManager>
#include <QUrl>

class QSslError;
class QAuthenticator;
class QNetworkReply;
class QTimer;

// TODO: namespace

class GBerryMainController : public QObject
{
    Q_OBJECT

public:
    // TODO: parent qobject?
    GBerryMainController();
    ~GBerryMainController();

    void start();
    void startRequest(QUrl url);

private slots:
    void timerUpdate();
    void httpFinished();
    void httpReadyRead();

//    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
//#ifndef QT_NO_SSL
//    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);
//#endif

private:
    // TODO: naming issues, style guide
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QTimer *timer;
};

#endif // GBERRYMAINCONTROLLER_H




