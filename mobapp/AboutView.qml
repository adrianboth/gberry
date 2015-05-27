import QtQuick 2.3
import QtQuick.Layouts 1.1

Item {
    id: self
    anchors.fill: parent

    // visually we are not filling parent fully,
    // there are margins on edges

    signal viewClosed()

    property string serverTextStr: "Server (INIT)"
    property string consoleTextStr: "Console (INIT)"

    property string appNameText: "<b>GBerry MobApp</b>"
    property string appVersionText: "v0.01.1"
    property string genericAboutText: "<html>This is general purpose GBerry
mobile application that is used together with GBerry console. See
<a href=\"http://gberry.xyz\">gberry.xyz</a> for more information.
<html/>
"

    Rectangle {
        id: view
        color: "snow"
        anchors.centerIn: parent
        anchors.margins: border.width

        width: parent.width - gdisplay.touchCellWidth() / 4
        height: parent.height - gdisplay.touchCellHeight() / 4
        radius: 10
        border.width: 1
        border.color: "lightgray"

        ColumnLayout {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            ViewTitleBar {
                id: titleBar
                titleText: qsTr("About")
                Layout.preferredWidth: view.width - 2 * view.anchors.margins
                Layout.preferredHeight: preferredHeight

                onCloseSelected: viewClosed()
            }

            // divider between titlebar and other content
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                color: "darkgrey"
            }

            // content area
            /*
            Item {
                Layout.fillWidth: true
                Layout.height: gdisplay.touchCellHeight() / 2
            }
            */

            Item {
                Layout.preferredWidth: view.width - 2 * view.anchors.margins

                Column {
                    anchors.top: parent.top
                    anchors.topMargin: gdisplay.tinySizeText

                    anchors.left: parent.left
                    anchors.leftMargin: gdisplay.tinySizeText

                    Text {
                        text: appNameText
                        height: implicitHeight
                        font.pixelSize: gdisplay.tinySizeText * 1.5
                    }

                    Text {
                        text: appVersionText
                        height: implicitHeight
                        font.pixelSize: gdisplay.tinySizeText
                    }

                    Item {
                        height: gdisplay.tinySizeText
                        width: 1 // without width is unvisible
                    }

                    Text {
                        // for text wrapping to work
                        width: view.width - 2 * view.anchors.margins - gdisplay.touchCellHeight() / 2

                        text: genericAboutText
                        wrapMode: Text.WordWrap
                        font.pixelSize: gdisplay.tinySizeText

                        onLinkActivated: {
                            console.debug("LINK ACTIVATED: " + link)
                            Qt.openUrlExternally(link)
                        }
                    }
                }

            }
        }
    }

}
