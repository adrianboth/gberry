import QtQuick 2.0

import GBerryConsole 1.0

Item {
    id: self

    property var enabledControlActions: ["Up", "Down", "OK"] // defaults

    function processControlAction(action) {
        if (action === "Up")
            mainMenu.moveFocusToPrevious()
        else if (action === "Down")
            mainMenu.moveFocusToNext()
        else if (action === "OK")
            mainMenu.selectCurrent()
    }


    property alias menuItems: mainMenu.items
    MainMenu {
        id: mainMenu
        anchors.centerIn: parent

    }
}

