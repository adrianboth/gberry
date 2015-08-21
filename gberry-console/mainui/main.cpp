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
 
#include <QGuiApplication>
#include <QProcessEnvironment>

#include <client/applicationmain.h>
#include <client/consoleapplication.h>

using namespace GBerryApplication;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ApplicationMain main(&app);

    ConsoleApplication consoleApp;
    if (main.hasApplicationCode())
        consoleApp.setApplicationCode(main.applicationCode());

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("FIRST_STARTUP") && env.value("FIRST_STARTUP") == "1") {
        consoleApp.setQmlContextProperty("FirstTimeStartup", QVariant(true));
    } else {
        consoleApp.setQmlContextProperty("FirstTimeStartup", QVariant(false));
    }

#ifdef GBERRY_DEBUG_QML_IMPORT_PATH
    consoleApp.setImportPaths(QString(xstr(GBERRY_DEBUG_QML_IMPORT_PATH)));
#endif

    consoleApp.run("qrc:/main.qml");

    return main.exec();
}
