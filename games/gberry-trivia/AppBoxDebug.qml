import QtQuick 2.0
import QtQuick.Layouts 1.1

import "appbox"

Item {
    id: self

    property string playerName: "unknown"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: "gray"

            Layout.fillWidth: true
            // TODO: there is no exact calculation to calculate topbar
            Layout.preferredHeight: gdisplay.smallSizeText * 1.2

            Text {
                text: playerName
                anchors.centerIn: parent
                font.pixelSize: gdisplay.smallSizeText
                color: "white"
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: appboxDebugWindow.height - titleBar.height

            AppBox {
                id: appbox
                property int playerId: 0

                enabled: true

                onOutgoingMessage: {
                    //console.debug("DEBUG CLIENT: onOutgoingMessage()for " + playerName)
                    // this message from appbox to applicaton
                    var js = {action: "AppBoxMessage", data: data}
                    playersManager.sendDebugPlayerMessage(playerId, JSON.stringify(js))
                }

                function onDebugPlayerMessageReceived(playerId_, data) {
                    //console.debug("### debug player message2: id = " + playerId + ", data = " + data)
                    if (playerId_ === playerId) {
                        console.debug("### debug player message: id = " + playerId + ", data = " + data)

                        // this is message from application to appbox
                        var js = JSON.parse(data)

                        if (js["action"] === "AppBoxMessage") {
                            //console.debug("DEBUG CLIENT: onDebugPlayerMessageReceived() for " + playerName)
                            incomingMessage(js["data"])
                        }
                    }
                }

                Component.onCompleted: {
                    console.debug("### REGISTERING DEBUG PLAYER " + playerName)
                    playerId = playersManager.registerDebugPlayer(self.playerName)
                    playersManager.debugPlayerMessageReceived.connect(onDebugPlayerMessageReceived)
                    console.debug("### pid: " + playerId)
                }
            }
        }
    }
}
