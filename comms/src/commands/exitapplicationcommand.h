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
