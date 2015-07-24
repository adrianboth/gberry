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
 
 #ifndef DOWNLOADMODEL_H
#define DOWNLOADMODEL_H

#include <QObject>
#include <QStringList>

namespace GBerry {

// TODO: Maybe there could be a common base class for communication objects ...
class IDownloadModelCommunication : public QObject
{
    Q_OBJECT
public:
    explicit IDownloadModelCommunication(QObject *parent = 0) : QObject(parent) {}
    virtual ~IDownloadModelCommunication() {}

    virtual void sendMessage(const QJsonObject& msg) = 0;

signals:
    //void messageReceived(const QJsonObject msg);
    void downloadStarted(QString applicationFullId);
    void downloadAborted(QString applicationFullId);
    void downloadFinished(QString applicationFullId);
    void downloadProgress(QString applicationFullId, int progressPercentage);
};


class DownloadModelPrivate;

/**
 * DownloadModel is used to start a download. There can be multiple
 * download requests, but 'comms' can limit one active at the time.
 *
 * When download has been requested it can get two responses
 *  - abort: if download can proceed from some reason (for example connection problems)
 *  - started: if downloading has been initiated succesfully and bytes are flowing in :-)
 *
 * After 'started' following events can occur
 *  - progress: telling that download is progressing
 *  - abort: telling that download has failed for some reason
 *  - finished: everything is done
 */
class DownloadModel : public QObject
{
    Q_OBJECT

public:
    explicit DownloadModel(IDownloadModelCommunication* comm, QObject *parent = 0);
    ~DownloadModel();

    // TODO: would download() be better name?
    Q_INVOKABLE void startDownload(QString applicationFullId);

    Q_INVOKABLE void cancelDownload(QString applicationFullId);

    // return applicationFullIds
    Q_INVOKABLE QStringList ongoingDownloads();

    // TODO: resume
    // TODO: pause

signals:
    // when startDownload() was called
    void downloadInitiated(QString applicationFullId);

    // download stream initiated
    void downloadStarted(QString applicationFullId);

    // in case of errors (TODO: later possiblity to resume ...)
    void downloadAborted(QString applicationFullId);

    // download completed successfully
    void downloadFinished(QString applicationFullId);

    void downloadProgress(QString applicationFullId, int progressPercentage);

public slots:
    /*
    void onDownloadStarted(const QString& applicationFullId);
    void onDownloadAborted(const QString& applicationFullId);
    void onDownloadFinished(const QString& applicationFullId);
    void onDownloadProgess(const QString& applicationFullId, int progressPercentage);
    */
private:
    const QScopedPointer<DownloadModelPrivate> _d;
};

} // eon

#endif // DOWNLOADMODEL_H
