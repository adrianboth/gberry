/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #ifndef PROXYQNETWORK_H
#define PROXYQNETWORK_H

#include <QNetworkAccessManager>

#include <QNetworkReply>


class ProxyQNetworkReply : public QObject
{
    Q_OBJECT

public:
    ProxyQNetworkReply(QNetworkReply* reply) : _reply(reply)
    {
        if (_reply)
        {
            // TODO: we could also use C++11 lambdas here, see Qt signal&slot doc
            connect(_reply, &QNetworkReply::metaDataChanged,
                    this,   &ProxyQNetworkReply::triggerMetaDataChanged);

            connect(_reply, &QNetworkReply::finished,
                    this,   &ProxyQNetworkReply::triggerFinished);

            // new style connect can't find right signal as there is error()
            // non-signal function. Using old style
            //connect(_reply, &QNetworkReply::error,
            //        this,   &ProxyQNetworkReply::triggerError);
            connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                    this,   SLOT(triggerError(QNetworkReply::NetworkError)));

          #ifndef QT_NO_SSL
            connect(_reply, &QNetworkReply::encrypted,
                    this,   &ProxyQNetworkReply::triggerEncrypted);

            connect(_reply, &QNetworkReply::sslErrors,
                    this,   &ProxyQNetworkReply::triggerSslErrors);
          #endif

            connect(_reply, &QNetworkReply::uploadProgress,
                    this,   &ProxyQNetworkReply::triggerUploadProgress);

            connect(_reply, &QNetworkReply::downloadProgress,
                    this,   &ProxyQNetworkReply::triggerDownloadProgress);

        }
    }

    // TODO: deletion

    virtual void close() { _reply->close(); }
    virtual bool isSequential() const { return _reply->isSequential(); }

    virtual qint64 readBufferSize() const { return _reply->readBufferSize(); }
    virtual void setReadBufferSize(qint64 size) { _reply->setReadBufferSize(size); }

    virtual QNetworkAccessManager *manager() const { return _reply->manager(); }
    virtual QNetworkAccessManager::Operation operation() const { return _reply->operation(); }
    virtual QNetworkRequest request() const { return _reply->request(); }
    virtual QNetworkReply::NetworkError error() const { return _reply->error(); }
    virtual bool isFinished() const { return _reply->isFinished(); }
    virtual bool isRunning() const { return _reply->isRunning(); }
    virtual QUrl url() const { return _reply-> url(); }

    virtual QVariant header(QNetworkRequest::KnownHeaders header) const { return _reply->header(header); }

    virtual bool hasRawHeader(const QByteArray &headerName) const { return _reply->hasRawHeader(headerName); }
    virtual QList<QByteArray> rawHeaderList() const { return _reply->rawHeaderList(); }
    virtual QByteArray rawHeader(const QByteArray &headerName) const { return _reply->rawHeader(headerName); }

    //typedef QPair<QByteArray, QByteArray> RawHeaderPair;
    virtual const QList<QNetworkReply::RawHeaderPair>& rawHeaderPairs() const { return _reply->rawHeaderPairs(); }

    virtual QVariant attribute(QNetworkRequest::Attribute code) const { return _reply->attribute(code); }

#ifndef QT_NO_SSL
    virtual QSslConfiguration sslConfiguration() const { return _reply->sslConfiguration(); }
    virtual void setSslConfiguration(const QSslConfiguration &configuration) { _reply->setSslConfiguration(configuration); }
    virtual void ignoreSslErrors(const QList<QSslError> &errors) { _reply->ignoreSslErrors(errors); }
#endif

public Q_SLOTS:
    virtual void abort() { _reply->abort(); }
    virtual void ignoreSslErrors() { _reply->ignoreSslErrors(); }

    // from QIODevice
    virtual QString errorString() const { return _reply->errorString(); }
    virtual QByteArray readAll() { return _reply->readAll(); }

    // proxy methods for signals
    // (these are public to enable unit testing)
    virtual void triggerMetaDataChanged() { emit metaDataChanged(); }
    virtual void triggerFinished() { emit finished(); }
    virtual void triggerError(QNetworkReply::NetworkError err) { emit error(err); }
#ifndef QT_NO_SSL
    virtual void triggerEncrypted() { emit encrypted(); }
    virtual void triggerSslErrors(const QList<QSslError> &errors) { emit sslErrors(errors); }
#endif
    void triggerUploadProgress(qint64 bytesSent, qint64 bytesTotal) { emit uploadProgress(bytesSent, bytesTotal); }
    void triggerDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) { emit downloadProgress(bytesReceived, bytesTotal); }

Q_SIGNALS:
    void metaDataChanged();
    void finished();
    void error(QNetworkReply::NetworkError);
#ifndef QT_NO_SSL
    void encrypted();
    void sslErrors(const QList<QSslError> &errors);
#endif
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    QNetworkReply* _reply;
};


class ProxyQNetworkAccessManager : public QObject
{
    Q_OBJECT

public:
    ProxyQNetworkAccessManager(QNetworkAccessManager* qnam) : _qnam(qnam) {}
    ProxyQNetworkAccessManager()
    {
        // desctructor will take care of deletion
        _qnam = new QNetworkAccessManager(this);
    }

    virtual ProxyQNetworkReply *get(const QNetworkRequest &request)
    {
        ProxyQNetworkReply* proxy = new ProxyQNetworkReply(_qnam->get(request));
        return proxy;
    }

private:
    QNetworkAccessManager* _qnam;
};


#endif // PROXYQNETWORK_H

