#ifndef MOCK_QNETWORKACCESSMANAGER_H
#define MOCK_QNETWORKACCESSMANAGER_H

template <class NetworkAccessManager>
NetworkAccessManager* CreateNetworkAccessManager(NetworkAccessManager* nam)
{
    // TODO: is possible to mock constructor ... to pass parent QObject
    //return new NetworkAccessManager();
    return nam;
}

template <class PacketStream>
class PacketReader {
 public:
  void ReadPackets(PacketStream* stream);

 private:
  PacketStream* packetstream;
};

//#include <QNetworkAccessManager>

//template class PacketReader<QNetworkAccessManager>;

//template <class PacketStream>
//void PacketReader<PacketStream>::ReadPackets(PacketStream *stream)
//{
//    // nothing
//}

#endif // MOCK_QNETWORKACCESSMANAGER_H

/**
//    explicit QNetworkAccessManager(QObject *parent = 0);
//    ~QNetworkAccessManager();

//    QStringList supportedSchemes() const;

//    void clearAccessCache();

#ifndef QT_NO_NETWORKPROXY
//    QNetworkProxy proxy() const;
//    void setProxy(const QNetworkProxy &proxy);
//    QNetworkProxyFactory *proxyFactory() const;
//    void setProxyFactory(QNetworkProxyFactory *factory);
#endif

//    QAbstractNetworkCache *cache() const;
//    void setCache(QAbstractNetworkCache *cache);

//    QNetworkCookieJar *cookieJar() const;
//    void setCookieJar(QNetworkCookieJar *cookieJar);

    QNetworkReply *head(const QNetworkRequest &request);
    QNetworkReply *get(const QNetworkRequest &request);
    QNetworkReply *post(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *post(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *put(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *deleteResource(const QNetworkRequest &request);
    QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QIODevice *data = 0);

#ifndef QT_NO_BEARERMANAGEMENT
    void setConfiguration(const QNetworkConfiguration &config);
    QNetworkConfiguration configuration() const;
    QNetworkConfiguration activeConfiguration() const;

    void setNetworkAccessible(NetworkAccessibility accessible);
    NetworkAccessibility networkAccessible() const;
#endif

#ifndef QT_NO_SSL
    void connectToHostEncrypted(const QString &hostName, quint16 port = 443,
                                const QSslConfiguration &sslConfiguration = QSslConfiguration::defaultConfiguration());
#endif
    void connectToHost(const QString &hostName, quint16 port = 80);

Q_SIGNALS:
#ifndef QT_NO_NETWORKPROXY
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
#endif
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void finished(QNetworkReply *reply);
#ifndef QT_NO_SSL
    void encrypted(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

#ifndef QT_NO_BEARERMANAGEMENT
    void networkSessionConnected();

    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
#endif

protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request,
                                         QIODevice *outgoingData = 0);

protected Q_SLOTS:
    QStringList supportedSchemesImplementation() const;

private:
    friend class QNetworkReplyImplPrivate;
    friend class QNetworkReplyHttpImpl;
    friend class QNetworkReplyHttpImplPrivate;

    Q_DECLARE_PRIVATE(QNetworkAccessManager)
    Q_PRIVATE_SLOT(d_func(), void _q_replyFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_replyEncrypted())
    Q_PRIVATE_SLOT(d_func(), void _q_replySslErrors(QList<QSslError>))
#ifndef QT_NO_BEARERMANAGEMENT
    Q_PRIVATE_SLOT(d_func(), void _q_networkSessionClosed())
    Q_PRIVATE_SLOT(d_func(), void _q_networkSessionStateChanged(QNetworkSession::State))
    Q_PRIVATE_SLOT(d_func(), void _q_onlineStateChanged(bool))
#endif
};
**/
