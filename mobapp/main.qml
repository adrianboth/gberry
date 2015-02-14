import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    MainForm {
        id: ui
        anchors.fill: parent
        // TODO: not sure if this right way to configure ui side
        property string serverTextStr: "Server (INIT)"
        property string consoleTextStr: "Console (INIT)"
        mouseArea.onClicked: {
            Qt.quit();
        }
    }

    function updateTexts()
    {
        ui.serverTextStr = "Server " + (app.serverConnectionOK ? "OK" : "NOK") + ": ok=" + app.serverPingOKCounter + " nok=" + app.serverPingFailureCounter
        ui.consoleTextStr = "Console " + (app.consoleConnectionOK ? "OK" : "NOK") + ": ok=" + app.consolePingOKCounter + " nok=" + app.consolePingFailureCounter
    }

    Component.onCompleted: {
        updateTexts()
        app.serverCounterChanged.connect(updateTexts)
        app.serverStatusChanged.connect(updateTexts)
        app.consoleCounterChanged.connect(updateTexts)
        app.consoleStatusChanged.connect(updateTexts)
    }
}
