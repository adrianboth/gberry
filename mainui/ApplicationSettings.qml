import QtQuick 2.0

import "gb:/js/DeveloperLog.js" as Log

Item {
    property int logLevel: Log.DEBUG_LEVEL

    // can be used during development enable helper functionality
    property bool developmentMode: true

}
