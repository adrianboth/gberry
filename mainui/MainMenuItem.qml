import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id: item

    // public
    property string text: "<undefined>"

    signal selected()

    // expects parent to have 'textPixelHeight' -> common for all menu items
    property int textPixelHeight: parent.textPixelHeight
    property int maxTextWidth: parent.maxTextWidth
    property int itemHeight: parent.itemHeight

    // we need to our own focus marker as we are controlling outside the current focus
    property bool focused: false

    // private
    color: "#2db6e1"

    Layout.alignment: Qt.AlignCenter
    Layout.preferredWidth: maxTextWidth + itemHeight * 0.25 * 2
    Layout.preferredHeight: itemHeight
    radius: 20 // TODO: size scaling
    border.width: focused ? 5: 0 // TODO: size scaling
    border.color: "black"

    // width is accessed outside to calculate max width from entries
    implicitWidth: itemText.implicitWidth

    Text {
        id: itemText
        text: item.text
        anchors.centerIn: parent
        font.pixelSize: textPixelHeight
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: selected()
    }
}
