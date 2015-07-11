#ifndef DOWNLOADAPPLICATIONCOMMAND_H
#define DOWNLOADAPPLICATIONCOMMAND_H

#include <QScopedPointer>
#include <QJsonObject>
#include <QProcess>

#include <server/icommand.h>

class ServerSideControlChannel;
class LocalApplicationsStorage;

namespace GBerryComms {
    class DownloadableApplicationCache;
}
using namespace GBerryComms;

namespace GBerry {

class HeadServerConnection;
class DownloadApplicationCommandPrivate;
class DownloadApplicationRequest;
class UnzipOperation;

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
    explicit DownloadApplicationCommand(
            HeadServerConnection* headServerConnection,
            ServerSideControlChannel* controlChannel,
            DownloadableApplicationCache* applicationCache,
            LocalApplicationsStorage* applicationsStorage);
    virtual ~DownloadApplicationCommand();

    // ICommand
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;


    // --

    // callback from Request
    void processRequestOkResponse(DownloadApplicationRequest* request);
    void processRequestErrorResponse(DownloadApplicationRequest* request);

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
