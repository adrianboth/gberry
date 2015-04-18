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

    RowLayout {
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

