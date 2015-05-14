import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

import GBerry 1.0

Rectangle {
    id: self
    anchors.fill: parent

    property string hostNameToConnect: "<undefined>"

    signal connectToConsoleRequested(var hostName)

    border.color: "slategray"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "slategray" }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: gdisplay.mediumSize * gdisplay.ppmText * 0.5

        Item {
            //color: "green"
            Layout.fillWidth: true
            Layout.preferredHeight: hostNameLabel.implicitHeight

            Text {
                id: hostNameLabel
                text: hostNameToConnect
                anchors.centerIn: parent
                font.pixelSize: gdisplay.mediumSize * gdisplay.ppmText
            }
        }

        Item {
            //color: "red"
            Layout.fillWidth: true
            Layout.preferredHeight: connectButton.height
            Layout.preferredWidth: connectButton.width

            GButton {
                id: connectButton
                width: buttonWidth
                height: buttonHeight

                label: qsTr("Connect")
                anchors.centerIn: parent
                onButtonClicked: {
                    console.log("CONNECT TO " + self.hostNameToConnect)
                    connectToConsoleRequested(self.hostNameToConnect)
                }
            }
        }
    }
/*
    ListModel {
        id: serverModel
        ListElement { text: "localhost" }
    }
    */
}

