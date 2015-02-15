#ifndef PROXYQNETWORK_H
#define PROXYQNETWORK_H

#include <QNetworkAccessManager>

#include <QNetworkReply>


class ProxyQNetworkReply
{
public:
    ProxyQNetworkReply(QNetworkReply* reply) : _reply(reply) {}

    virtual bool isFinished() const { return _reply->isFinished(); }

private:
    QNetworkReply* _reply;

};


class ProxyQNetworkAccessManager
{
public:
    ProxyQNetworkAccessManager(QNetworkAccessManager* qnam) : _qnam(qnam) {}

    virtual QNetworkReply *get(const QNetworkRequest &request)
    {
        return _qnam->get(request);
    }
    virtual ProxyQNetworkReply *get_proxytest(const QNetworkRequest &request)
    {
        //_qnam->get(request);
        // TODO: memory handling : - how QNetworkReply is typically deleted -> deleteLater()
        ProxyQNetworkReply* proxy = new ProxyQNetworkReply(_qnam->get(request));
        return proxy;
        return NULL;
    }

private:
    QNetworkAccessManager* _qnam;
};


#endif // PROXYQNETWORK_H

