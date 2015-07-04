#include "downloadmodel.h"

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
                         [&] (const String& applicationFullId) {
            q->downloadStarted(applicationFullId, progressPercentage);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadAborted,
                         [&] (const String& applicationFullId) {
            ongoingDownloads.removeOne(applicationFullId);
            emit q->downloadAborted(applicationFullId);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadFinished,
                         [&] (const String& applicationFullId) {
            ongoingDownloads.removeOne(applicationFullId);
            emit q->downloadFinished(applicationFullId);
        });

        QObject::connect(comm, &IDownloadModelCommunication::downloadProgress,
                         [&] (const String& applicationFullId, int progressPercentage) {
             q->downloadProgress(applicationFullId, progressPercentage);
        });

    }

    DownloadModel q;
    IDownloadModelCommunication comm;
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
    QJsonObject json;
    json["command"]  = "DownloadApplication";
    json["application_id"] = applicationFullId;
    _d->comm->sendMessage(json);
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
