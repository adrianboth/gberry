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
 
 #ifndef LOGCONTROL_H
#define LOGCONTROL_H

#include <QObject>
#include <QList>

// forward declarations
class ILogMsgHandler;

class LogControl : public QObject
{
    Q_OBJECT

public:
    LogControl();
    ~LogControl();
    // TODO: copy constructors and such

    void registerMsgHandler(ILogMsgHandler* msgHandler);

    QList<ILogMsgHandler*> handlers(const QString& logArea) const;

    int highestLogLevel() const { return _highestLogLevel; }

signals:
    void highestLogLevelChanged();

private:
    class PrivateLogControlImpl;
    PrivateLogControlImpl* _impl;
    int _highestLogLevel;
};

#endif // LOGCONTROL_H
