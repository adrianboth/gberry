/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 import QtQuick 2.0

Item {
    id: self
    width: actionMenu.width; height: actionMenu.height

    property int maxHeight: 0 // not defined
    property int maxWidth: 0 // not defined

    property bool hasActions: listModel.count > 0

    signal actionSelected(var actionId)

    // private
    property real buttonScaler: 1.0

    onVisibleChanged: {
        if (visible)
            calcScaling()
    }

    onMaxHeightChanged: { calcScaling() }
    onMaxWidthChanged: { calcScaling() }

    function calcScaling() {

        if (visible) {
            console.log("############### CALC GeneralActions height")
            // how to make items smaller?
            //  -> decrease margin and buttons proportionally until ok
            //     -> use scaler

            // only if max have been defined
            var buttonScalerCandinate = 0
            if (maxHeight !== 0) {
                var heightCandinate = actionMenu.defaultHeight
                console.debug("### maxHeight: " + maxHeight + ", heightCandinate: " + heightCandinate)
                if (heightCandinate > maxHeight) {
                    buttonScalerCandinate = maxHeight / heightCandinate

                } else if (maxHeight / heightCandinate > buttonScaler) {

                    buttonScalerCandinate = maxHeight / heightCandinate
                }

                console.debug("### buttonScalerCandinate after height calc: " + buttonScalerCandinate)

            }

            if (maxWidth !== 0) {
                console.debug("maxWidht: " + maxWidth + ", actionMenu.defaultWidth: " + actionMenu.defaultWidth)
                var widthCandinate = actionMenu.defaultWidth // contains already effect of current scaler
                if (widthCandinate > maxWidth) {

                    var widthScalerCandinate = maxWidth / widthCandinate //* buttonScaler
                    if (buttonScalerCandinate === 0 || widthScalerCandinate < buttonScalerCandinate) {
                        buttonScalerCandinate = widthScalerCandinate
                    } else if (widthScalerCandinate > buttonScaler) {
                        buttonScalerCandinate = widthScalerCandinate
                    }
                    console.debug("### buttonScalerCandinate after height calc: " + buttonScalerCandinate)
                } else {
                    var widthScalerCandinate = maxWidth / widthCandinate //* buttonScaler
                    if (buttonScalerCandinate === 0 || widthScalerCandinate < buttonScalerCandinate) {
                        buttonScalerCandinate = widthScalerCandinate
                    } else if (widthScalerCandinate > buttonScaler) {
                        buttonScalerCandinate = widthScalerCandinate
                    }
                }
            }

            if (buttonScalerCandinate > 0 && buttonScalerCandinate <= 1.0)
                buttonScaler = buttonScalerCandinate
        }
    }

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
        property int marginX: gdisplay.touchCellWidth() / 2 * buttonScaler
        //property int marginY: gdisplay.touchCellHeight() / 2
        property int marginY: border.width // to prevent drawing on top of border
        width: list.contentWidth + marginX * 2
        height: list.contentHeight + marginY * 2
        property int defaultHeight: listFrame.defaultHeight + marginY * 2
        property int defaultWidth: listFrame.defaultWidth + marginX * 2

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
            id: listFrame
            width: list.contentWidth
            height: list.contentHeight
            property int defaultHeight: defaultDelegateHeight * listModel.count
            property int defaultWidth: list.contentWidth
            anchors.centerIn: parent

            ListView {
                id: list
                anchors.fill: parent
                interactive: false
                contentWidth: contentItem.childrenRect.width;
                contentHeight: contentItem.childrenRect.height

                model: listModel
                delegate: actionDelegate
            }

            //property int defaultDelegateHeight: text.defaultHeight * 2 + upperEmptySpace.defaultHeight + divider.height + bottomEmptySpace.defaultHeight
            property int defaultDelegateHeight: gdisplay.smallSizeText * 2 + gdisplay.smallSizeText/2 + 2 + gdisplay.smallSizeText/2



            Component {
                id: actionDelegate

                Rectangle {
                    width: text.implicitWidth * buttonScaler
                    height: (text.implicitHeight + divider.height + gdisplay.smallSizeText) * buttonScaler
                    //height: defaultHeight * buttonScaler
                    //property int defaultHeight: text.defaultHeight * 2 + upperEmptySpace.defaultHeight + divider.height + bottomEmptySpace.defaultHeight
                    color: "snow"
                    //color: "red"
                    property string id: actionId

                    Item {
                        id: upperEmptySpace
                        property int defaultHeight: gdisplay.smallSizeText/2
                        height: defaultHeight * buttonScaler
                        width: 1
                        //color: "green"
                    }

                    Text {
                        id: text
                        color: "black"
                        text: actionName
                        font.pointSize: gdisplay.smallSizeText * buttonScaler
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: upperEmptySpace.bottom
                        property int defaultHeight: gdisplay.smallSizeText
                    }

                    Item {
                        id: bottomEmptySpace
                        height: defaultHeight *buttonScaler
                        property int defaultHeight: gdisplay.smallSizeText/2
                        width: 1
                        //color: "green"
                        anchors.top: text.bottom
                    }

                    Rectangle {
                        id: divider
                        // last item should not have divider
                        visible: index != listModel.count -1
                        height: 2
                        width: 0.7 * actionMenu.width * buttonScaler// 0.15+0.15 for left/right margins
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
