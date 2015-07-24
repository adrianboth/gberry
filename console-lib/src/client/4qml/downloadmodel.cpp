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
 
 #include "downloadmodel.h"

#include <QJsonObject>

#define LOG_AREA "DownloadModel"
#include <log/log.h>

namespace GBerry {

class DownloadModelPrivate
{
public:
    DownloadModelPrivate(IDownloadModelCommunication* comm_, DownloadModel* q_) :
        q(q_),
        comm(comm_) {

        QObject::connect(comm, &IDownloadModelCommunication::downloadStarted,
                         [&] (const QString& applicationFullId) {
            q->downloadStarted(applicationFullId);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadAborted,
                         [&] (const QString& applicationFullId) {
            ongoingDownloads.removeOne(applicationFullId);
            emit q->downloadAborted(applicationFullId);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadFinished,
                         [&] (const QString& applicationFullId) {
            ongoingDownloads.removeOne(applicationFullId);
            emit q->downloadFinished(applicationFullId);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadProgress,
                         [&] (const QString& applicationFullId, int progressPercentage) {
             q->downloadProgress(applicationFullId, progressPercentage);
        });

    }

    DownloadModel* q;
    IDownloadModelCommunication* comm;
    QStringList ongoingDownloads;
};


DownloadModel::DownloadModel(IDownloadModelCommunication* comm, QObject *parent) :
    QObject(parent),
    _d(new DownloadModelPrivate(comm, this))
{
}

DownloadModel::~DownloadModel()
{
}

void DownloadModel::startDownload(QString applicationFullId)
{
    _d->ongoingDownloads.append(applicationFullId);
    QJsonObject json;
    json["command"]  = "DownloadApplication";
    json["application_id"] = applicationFullId;
    _d->comm->sendMessage(json);
    emit downloadInitiated(applicationFullId);
}

void DownloadModel::cancelDownload(QString applicationFullId)
{
    QJsonObject json;
    json["command"]  = "CancelDownloadApplication";
    json["application_id"] = applicationFullId;
    _d->comm->sendMessage(json);
}

QStringList DownloadModel::ongoingDownloads()
{
    return _d->ongoingDownloads;
}

} // eon
