import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    color: "orange"

    function setNumbers(listOfNumbers) {
        // TODO: assuming there is four
        firstbox.number = listOfNumbers[0]
        secondbox.number = listOfNumbers[1]
        thirdbox.number = listOfNumbers[2]
        fourthbox.number = listOfNumbers[3]
    }

    // relative coords
    function xForLeftMostBox() {
        return boxline.x + firstbox.x
    }
    // relative coords
    function yForBoxLine() {
        return boxline.y + firstbox.y
    }
    function stepSize() {
        // box width + margin
        return firstbox.width + boxline.spacing
    }

    RowLayout {
        id: boxline
        anchors.centerIn: parent
        spacing: gdisplay.largeSize * gdisplay.ppmText

        NumberBox {
            id: firstbox
        }

        NumberBox {
            id: secondbox
        }

        NumberBox {
            id: thirdbox
        }

        NumberBox {
            id: fourthbox
        }
    }
}

