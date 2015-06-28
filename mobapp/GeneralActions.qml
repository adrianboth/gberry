import QtQuick 2.0

Item {
    id: self
    width: actionMenu.width; height: actionMenu.height

    property bool hasActions: listModel.count > 0

    signal actionSelected(var actionId)

    function clearActions() {
        listModel.clear()
    }

    function setActions(actionList) {
        listModel.clear()
        actionList.map(function(actionJson) {
            if (actionJson.hasOwnProperty("actionName") && actionJson.hasOwnProperty("actionId")) {
                console.debug("Appending action: " + actionJson["actionName"])
                listModel.append({actionName: actionJson["actionName"],
                                  actionId: actionJson["actionId"]})
            } else {
                console.debug("Discard invalid action definition:" + actionJson.toString())
            }


        })
    }

    ListModel {
        id: listModel
    }

    Rectangle {
        id: actionMenu
        property int marginX: gdisplay.touchCellWidth() / 2
        //property int marginY: gdisplay.touchCellHeight() / 2
        property int marginY: border.width // to prevent drawing on top of border
        width: list.contentWidth + marginX * 2
        height: list.contentHeight + marginY * 2
        color: "snow"
        border.width: 1
        border.color: "lightgrey"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                //console.debug("ACTION MENU MOUSEAREA")
                //console.debug("MouseArea::clicked " + mouse.x + "," + mouse.y)

                // trick is to find item on that row (actual text item doesn't fill the full row)
                // also take space of margins out
                var x = 0 // just on ListView item side
                var y = mouse.y - actionMenu.marginY

                // clicking upper margin area is same as selecting first item
                // clicking bottom margin area is same as selecting last item
                if ( y < 0 )
                    y = 0
                else if (y > list.contentHeight)
                    y = list.contentHeight

                //console.debug("ListView " + x + "," + y)
                var item = list.itemAt(x, y)
                if (item !== null) {
                    //console.debug("item: " + item.toString())
                    console.debug("item id: " + item.id)
                    actionSelected(item.id)
                }
            }
        }

        // additional item needed as centerIn for ListView doesn't work properly ...
        Item {
            width: list.contentWidth
            height: list.contentHeight
            anchors.centerIn: parent

            ListView {
                id: list
                anchors.fill: parent
                interactive: false
                contentWidth: contentItem.childrenRect.width; contentHeight: contentItem.childrenRect.height

                model: listModel
                delegate: actionDelegate
            }

            Component {
                id: actionDelegate

                Rectangle {
                    width: text.implicitWidth
                    height: text.implicitHeight + divider.height + gdisplay.smallSizeText
                    color: "snow"
                    //color: "red"
                    property string id: actionId

                    Item {
                        id: upperEmptySpace
                        height: gdisplay.smallSizeText/2
                        width: 1
                        //color: "green"
                    }

                    Text {
                        id: text
                        color: "black"
                        text: actionName
                        font.pointSize: gdisplay.smallSizeText
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: upperEmptySpace.bottom
                    }

                    Item {
                        id: bottomEmptySpace
                        height: gdisplay.smallSizeText/2
                        width: 1
                        //color: "green"
                        anchors.top: text.bottom
                    }

                    Rectangle {
                        id: divider
                        // last item should not have divider
                        visible: index != listModel.count -1
                        height: 2
                        width: 0.7 * actionMenu.width // 0.15+0.15 for left/right margins
                        color: "lightgray"
                        anchors.top: bottomEmptySpace.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 0.15 * actionMenu.width - actionMenu.marginX
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        // test data
        //var js = [{actionName: "Menu", actionId: "Menu"}, {actionName: "Exit", actionId: "Exit"}]
        //setActions(js)
    }
}
