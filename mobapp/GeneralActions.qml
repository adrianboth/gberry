import QtQuick 2.0

Rectangle {
    id: actionMenu
    width: list.contentWidth + list.anchors.margins * 2
    height: list.contentHeight + list.anchors.margins * 2
    color: "snow"
    border.width: 1
    border.color: "lightgrey"

    property bool hasActions: listModel.count > 0

    signal actionSelected(var actionId)

    function setActions(actionList) {
        listModel.clear()
        actionList.map(function(actionJson) {
            console.debug("Appending action: " + actionJson["actionName"])
            listModel.append({actionName: actionJson["actionName"],
                              actionId: actionJson["actionId"]})
        })
    }

    ListModel {
        id: listModel
        /*
        ListElement {
               actionName: "Menu"
               actionId: "Menu"
        }
        ListElement {
               actionName: "Exit"
               actionId: "Exit"
        }
        */
    }

    Component {
            id: actionDelegate
            Rectangle {
                width: text.implicitWidth
                height: text.implicitHeight
                color: "snow"

                Text {
                    id: text
                    color: "black"
                    text: actionName
                    font.pointSize: gdisplay.mediumSize * gdisplay.ppmText
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: divider
                    // last item should not have divider
                    visible: index != listModel.count -1
                    height: 2
                    width: 0.7 * actionMenu.width // 0.15+0.15 for left/right margins
                    color: "lightgray"
                    anchors.top: text.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 0.15 * actionMenu.width
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: actionSelected(listModel.get(index).actionId)
                }
            }
        }

    ListView {
        id: list
        anchors.fill: parent
        anchors.margins: gdisplay.touchCellWidth() / 2

        interactive: false
        spacing: gdisplay.touchCellWidth() / 2

        contentWidth: contentItem.childrenRect.width; contentHeight: contentItem.childrenRect.height

        model: listModel
        delegate: actionDelegate
    }

    Component.onCompleted: {
        // test data
        //var js = [{actionName: "Menu", actionId: "Menu"}, {actionName: "Exit", actionId: "Exit"}]
        //setActions(js)
    }
}
