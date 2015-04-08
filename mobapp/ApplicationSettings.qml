import QtQuick 2.0

import GBerry 1.0 // 'Log' from here

Item {
    property int logLevel: Log.DEBUG_LEVEL

    // can be used during development enable helper functionality
    property bool developmentMode: true

    // color names: http://www.w3.org/TR/css3-color/#svg-color
    //property color elementBackground: "snow"
}
