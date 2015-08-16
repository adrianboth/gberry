import QtQuick 2.4
import QtQuick.Layouts 1.1

Item {
    id: self

    property string imagePath: currentQuestion.imagePath
    property string questionText: currentQuestion.text

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: gdisplay.touchCellHeight()

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: questionTextField.implicitHeight

            Text {
                id: questionTextField
                anchors.centerIn: parent

                font.pixelSize: gdisplay.mediumSizeText
                text: questionText
                wrapMode: Text.WordWrap
                // without specific width word wrapping doesn't work
                width: self.width - 2*gdisplay.touchCellHeight()
            }
        }

        Image {
            Layout.maximumWidth: 400
            Layout.maximumHeight: 200
            Layout.preferredWidth: sourceSize.width
            Layout.preferredHeight: sourceSize.height
            Layout.alignment: Qt.AlignHCenter
            fillMode: Image.PreserveAspectFit
            visible: imagePath != ""
            source: imagePath
        }
    }
}
