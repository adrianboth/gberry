import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id: item

    // public
    property string text: "<undefined>"

    signal selected()

    property int textPixelHeight: parent.textPixelHeight

    //property int maxTextWidth: 200 // temporary initial value
    // TODO: itemHeight
    //property int textPixelHeight: 16 // temporary initial value

    // private
    color: "#2db6e1"

    // TODO: ?
    //focus: true

    // we need to our own focus marker as we are controlling outside the current focus
    property bool focused: false

    Layout.alignment: Qt.AlignCenter
    Layout.preferredWidth: parent.maxTextWidth + parent.itemHeight * 0.25 * 2
    Layout.preferredHeight: parent.itemHeight
    radius: 20 // TODO: size scaling
    border.width: focused ? 5: 0 // TODO: size scaling
    border.color: "black"

    // width is accessed outside to calculate max width from entries
    implicitWidth: itemText.implicitWidth

    Text {
        id: itemText
        text: item.text
        anchors.centerIn: parent
        font.pixelSize: parent.textPixelHeight
        smooth: true

        // does these have any effect?
        style: Text.Raised
        styleColor: "transparent"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: selected()
    }
}
