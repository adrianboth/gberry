import QtQuick 2.0

Rectangle {
    width: 200
    height: listModel.count * 35
    color: "darkgrey"

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
                width: 200
                height: text.implicitHeight
                color: "darkgrey"
                border.width: 1
                border.color: "lightgrey"


                Text {
                    id: text
                    color: "white"
                    text: actionName
                    font.pointSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: actionSelected(listModel.get(index).actionId)
                }
            }
        }

    ListView {
        anchors.fill: parent
        anchors.leftMargin: 5

        model: listModel
        delegate: actionDelegate
    }

    Component.onCompleted: {
        // test data
        //var js = [{actionName: "Menu", actionId: "Menu"}, {actionName: "Exit", actionId: "Exit"}]
        //setActions(js)
    }
}

