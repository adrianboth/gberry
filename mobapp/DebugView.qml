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
    property string appVersionText: "v0.01"
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

        width: parent.width - gdisplay.touchCellWidth()
        height: parent.height - gdisplay.touchCellHeight()
        radius: 10
        border.width: 1
        border.color: "lightgray"

        ColumnLayout {
            anchors.top: parent.top
            spacing: 0


            Item {
                id: titleBar
                //Layout.fillWidth: true
                Layout.preferredWidth: view.width - 2 * view.anchors.margins
                Layout.preferredHeight: gdisplay.touchCellHeight() * 2
                //color: "red"
                //gradient: gsettings.titleGradient

                Text {
                    text: qsTr("About")
                    font.pixelSize: gdisplay.touchCellHeight()
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: gdisplay.touchCellHeight() / 2
                }

                Rectangle {
                    id: closeButton
                    width: closeButton.height
                    height: parent.height * 0.65
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: gdisplay.touchCellHeight() / 2
                    radius: 50 // circle
                    border.width: 1
                    border.color: "grey"
                    color: "lightgrey"

                    Canvas {
                        id: cross
                        // canvas size
                        width: parent.width; height: parent.height
                        // handler to override for drawing
                        property real crossLineMarginX: 0.3 * cross.width
                        property real crossLineMarginY: 0.3 * cross.height

                        onPaint: {
                            // get context to draw with
                            var ctx = getContext("2d")
                            // setup the stroke
                            ctx.lineWidth = 2
                            ctx.strokeStyle = "black"
                            // setup the fill
                            ctx.fillStyle = "steelblue"
                            // begin a new path to draw
                            ctx.beginPath()

                            ctx.moveTo(0 + crossLineMarginX, 0 + crossLineMarginY)
                            ctx.lineTo(cross.width - crossLineMarginX, cross.height - crossLineMarginY)
                            ctx.stroke()

                            ctx.beginPath()
                            ctx.moveTo(cross.width - crossLineMarginX, 0 + crossLineMarginY)
                            ctx.lineTo(0 + crossLineMarginX, cross.height - crossLineMarginY)
                            ctx.stroke()
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            self.viewClosed()
                        }
                    }
                }
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
                    anchors.topMargin: gdisplay.touchCellHeight() / 2

                    anchors.left: parent.left
                    anchors.leftMargin: gdisplay.touchCellHeight() / 2

                    Text {
                        text: appNameText
                        height: implicitHeight
                        font.pixelSize: gdisplay.mediumSizeText
                    }

                    Text {
                        text: appVersionText
                        height: implicitHeight
                        font.pixelSize: gdisplay.smallSizeText
                    }

                    Item {
                        height: gdisplay.touchCellHeight() / 2
                        width: 1 // without width is unvisible
                    }

                    Text {
                        // for text wrapping to work
                        width: view.width - 2 * view.anchors.margins - gdisplay.touchCellHeight()

                        text: genericAboutText
                        wrapMode: Text.WordWrap
                        font.pixelSize: gdisplay.smallSizeText

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
