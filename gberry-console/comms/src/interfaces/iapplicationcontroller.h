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
 
 #ifndef IAPPLICATIONCONTROLLER_H
#define IAPPLICATIONCONTROLLER_H

#include <QObject>

class IApplicationController : public QObject
{
    Q_OBJECT

public:
    IApplicationController(QObject* parent = 0) : QObject(parent) {}
    ~IApplicationController() {}

    virtual void launch() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    virtual QString fullApplicationId() const = 0;

signals:
    void launched();
    void launchFailed();
    void resumed();
    void paused();
    void resumeFailed();
    void stopped();
    void died();
};

#endif // IAPPLICATIONCONTROLLER_H
