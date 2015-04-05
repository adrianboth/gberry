import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    color: "snow"
    radius: 20
    antialiasing: true

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

    property color menuItemBgColor: "#2db6e1"
    property int menuItemRadius: 20

    ColumnLayout {
        id: columnLayout
        spacing: 2
        anchors.centerIn: parent
        anchors.fill: parent
        height: playGameMenuEntry.implicitHeight + exitGameMenuEntry.implicitHeight + 2*35

        Rectangle {
            id: playGameMenuEntry
            color: menuItemBgColor
            focus: true
            property bool focused: true

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 150
            Layout.preferredHeight: 40
            anchors.margins: 15
            radius: menuItemRadius
            border.width: focused ? 5: 0
            border.color: "black"

            Text {
                text: "Play Game"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 16
                smooth: true
            }

            function select() {
                playGameSelected()
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

        Rectangle {
            id: exitGameMenuEntry
            color: menuItemBgColor
            property bool focused: false
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 150
            Layout.preferredHeight: 40
            anchors.margins: 15
            radius: menuItemRadius
            antialiasing: true

            border.width: focused ? 5: 0
            border.color: "black"

            Text {
                text: "Exit Game"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 16
                smooth: true
            }

            function select() {
                exitGameSelected()
            }
        }
    }
}

