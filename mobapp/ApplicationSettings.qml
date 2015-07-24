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
 
 import QtQuick 2.0

//import GBerry 1.0 // 'Log' from here
import "ApplicationSettingsJS.js" as ApplicationSettingsJS

Item {
    property int logLevel: ApplicationSettingsJS.logLevel

    // can be used during development enable helper functionality
    property bool developmentMode: true

    // color names: http://www.w3.org/TR/css3-color/#svg-color
    //property color elementBackground: "snow"

    Gradient {
        id: titleGradient
        GradientStop { position: 0.0; color: "gray" }
        GradientStop { position: 1.0; color: "snow" }
    }
}
