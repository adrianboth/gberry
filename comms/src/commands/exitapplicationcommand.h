#ifndef EXITAPPLICATIONCOMMAND_H
#define EXITAPPLICATIONCOMMAND_H


#include <QObject>
#include <QJsonObject>

#include "server/icommand.h"
#include "server/application/iapplications.h"


namespace GBerry {
namespace Console {
namespace Server {
    class ApplicationRegistry;
}}}

namespace GBerryComms {

class ExitApplicationCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    ExitApplicationCommand(const QString& applicationId);
    ~ExitApplicationCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

signals:
    void exitApplicationRequested(const QString applicationId);

private:
    QString _applicationId;

};

} // eon


#endif // EXITAPPLICATIONCOMMAND_H
