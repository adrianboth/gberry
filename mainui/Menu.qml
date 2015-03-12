import QtQuick 2.0
import QtQuick.Layouts 1.1

// Implementation Notes:
//
// Menu has its own focus implementation because relaying on normal focus
// system doesn't work if you are developing locally, i.e. running both
// ends. The focus can't be in app while you are pressing mobapp.
//

Rectangle {
    id: root
    color: "green"
    width: 300
    height: columnLayout.implicitHeight + 50

    signal playGameSelected()
    signal exitGameSelected()

    function moveFocusToNext() {
        console.debug("Next focus")
       //root.nextItemInFocusChain(true)
        if (playGameMenuEntry.focused) {
            exitGameMenuEntry.focused = true
            playGameMenuEntry.focused = false

        } else {
            exitGameMenuEntry.focused = false
            playGameMenuEntry.focused = true
        }
    }

    function moveFocusToPrevious() {
        console.debug("Prev focus")
        //root.nextItemInFocusChain(false)
        if (playGameMenuEntry.focused) {
            exitGameMenuEntry.focused = true
            playGameMenuEntry.focused = false
        }
        else {
            exitGameMenuEntry.focused = false
            playGameMenuEntry.focused = true
        }
    }

    function selectCurrent()
    {
        console.debug("Select current")
        if (playGameMenuEntry.focused)
            playGameMenuEntry.select()
        if (exitGameMenuEntry.focused)
            exitGameMenuEntry.select()
    }

    /*
    FocusScope {
        id: scope
        //property alias colorx: columnLayout.color
        x: columnLayout.x
        y: columnLayout.y
        width: columnLayout.width
        height: columnLayout.height
        anchors.centerIn: parent
        anchors.fill: parent
*/
        ColumnLayout {
            id: columnLayout
            spacing: 2
            anchors.centerIn: parent
            anchors.fill: parent
            height: playGameMenuEntry.implicitHeight + exitGameMenuEntry.implicitHeight + 2*35

            Rectangle {
                id: playGameMenuEntry
                color: "blue"
                focus: true
                property bool focused: true

                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 150
                Layout.preferredHeight: 40
                anchors.margins: 15

                border.width: focused ? 5: 0
                border.color: "black"

                Text {
                    text: "Play Game"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                }

                function select() {
                    playGameSelected()
                }

                /*
                Keys.onPressed: {
                    console.debug("######## KEY PRESSED (play)")
                    if (event.key === Qt.Key_Up)
                        moveFocusToNext()
                    if (event.key === Qt.Key_Down)
                        moveFocusToPrevious()
                    if (event.key === Qt.Key_Return)
                        selectCurrent()
                }
                */
            }

            Rectangle {
                id: exitGameMenuEntry
                color: "red"
                property bool focused: false
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 150
                Layout.preferredHeight: 40
                anchors.margins: 15

                border.width: focused ? 5: 0
                border.color: "black"

                Text {
                    text: "Exit Game"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                }

                function select() {
                    exitGameSelected()
                }
                /*
                Keys.onPressed: {
                    console.debug("######## KEY PRESSED (exit)")
                    if (event.key === Qt.Key_Up)
                        moveFocusToNext()
                    if (event.key === Qt.Key_Down)
                        moveFocusToPrevious()
                    if (event.key === Qt.Key_Return)
                        selectCurrent()
                }
                */
            }
//        }
    }


    Keys.onPressed: {
        console.debug("######## KEY PRESSED (menu)")
        if (event.key === Qt.Key_Up)
            moveFocusToNext()
        if (event.key === Qt.Key_Down)
            moveFocusToPrevious()
        if (event.key === Qt.Key_Return)
            selectCurrent()
    }


    Component.onCompleted: {
        playGameMenuEntry.forceActiveFocus()
    }
}

