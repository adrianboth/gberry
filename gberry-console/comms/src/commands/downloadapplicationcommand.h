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
 
 #ifndef DOWNLOADAPPLICATIONCOMMAND_H
#define DOWNLOADAPPLICATIONCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>
#include <QProcess>

#include <server/icommand.h>
#include <result.h>
using namespace GBerryLib;


class ServerSideControlChannel;
class LocalApplicationsStorage;
class PlayerSessionManager;

namespace GBerryComms {
    class DownloadableApplicationCache;
}
using namespace GBerryComms;

namespace GBerry {

ERRORCLASS(DownloadApplicationCommandError)

class DownloadApplicationCommandErrors {
public:
    static const DownloadApplicationCommandError INTERNAL_ERROR;
};

class HeadServerConnection;
class DownloadApplicationCommandPrivate;
class DownloadApplicationRequest;
class UnzipOperation;
class DownloadStreamInvocation;

/**
 * @brief The DownloadApplicationCommand class
 *
 * There will be one instance of this class for each ControlChannel. This class
 * will create DownloadableApplicationRequest objects do handle individual
 * downloads.
 */
class DownloadApplicationCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    // TODO: how to create factory like
    explicit DownloadApplicationCommand(HeadServerConnection* headServerConnection,
            ServerSideControlChannel* controlChannel,
            DownloadableApplicationCache* applicationCache,
            LocalApplicationsStorage* applicationsStorage,
            PlayerSessionManager *playerSessions);
    virtual ~DownloadApplicationCommand();

    // ICommand
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;


    // --

    // callback from Request
    void processRequestOkResponse(DownloadApplicationRequest* request);
    void processRequestErrorResponse(DownloadApplicationRequest* request, const Result& result);
    void downloadStarted(DownloadApplicationRequest* request);
    void downloadProgress(DownloadApplicationRequest* request, DownloadStreamInvocation* inv);

protected:
    void onUnzipFinished(UnzipOperation* op);

private:
    const QScopedPointer<DownloadApplicationCommandPrivate> _d;

};

class UnzipOperation : public QObject
{
    Q_OBJECT
public:
    UnzipOperation(const QString& applicationFullId, const QString& zipFilePath);
    ~UnzipOperation();

    QString applicationFullId() const;
    QString zipFilePath() const;
    int exitCode() const;

    void execute();

public slots:
    void onQProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void finished(UnzipOperation* op);

private:
    QString _applicationFullId;
    QString _zipFilePath;
    int _exitCode{-1};
};

} // eon

#endif // DOWNLOADAPPLICATIONCOMMAND_H
