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
 
#ifndef APPLICATIONSTORAGE_H
#define APPLICATIONSTORAGE_H

#include <QQmlApplicationEngine>

#include "iapplicationstorage.h"

namespace GBerryClient {

/**
 * @brief The ApplicationStorage class
 *
 * This class manages file location where information can be saved. It is
 * targeted for mobile applications.
 *
 * By default the storage location is same as a default offline storage
 * location, but for development purposes it can be affected by an enviroment
 * variable APP_ID. In that case a subdirectory is created 'app_<id>'.
 *
 * The path is set for provided QQmlApplicationEngine.
 */
class ApplicationStorage : public IApplicationStorage
{
public:
    ApplicationStorage(QQmlApplicationEngine* engine);
    ~ApplicationStorage();

    QString storagePath() const override;

private:
    QQmlApplicationEngine* _engine;
};

} // eon

#endif // APPLICATIONSTORAGE_H
