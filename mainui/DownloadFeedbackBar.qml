import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id: self
    visible: false // by default hidden
    z: 1000 // tries to be always top most item

    // public
    property string applicationFullId: ""
    property string message: "<undefined>"
    property int showingTime: 3000 // ms
    property color bgColor: "snow"
    property real initialOpacity: 0.6
    property int textPixelSize: gdisplay.smallSize * gdisplay.ppmText
    property int preferredHeight: textarea.height

    function startDownload(gameFullId) {
        applicationFullId = gameFullId
        DownloadModel.startDownload(gameFullId)
        message = qsTr("Initiating download of ") + gameFullId
        visible = true
    }

    function close() {
        self.visible = false
        self.opacity = initialOpacity
    }


    // private
    property int textMargin: gdisplay.touchCellHeight() // from one text line height

    color: "snow"
    radius: 20 // TODO: should change based on screen size?
    opacity: initialOpacity

    Item {
        id: textarea
        height: textLabel.contentHeight + textMargin // margin is based on line height
        width: textLabel.width // + 2 * textMargin

        anchors.centerIn: parent

        Text {
            id: textLabel
            text: message

            y: parent.y + textMargin // top margin
            anchors.centerIn: parent

            font.bold: false
            font.pixelSize: textPixelSize

            // needed for text wrapping to work
            width: self.width * 0.85

            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }

    SequentialAnimation {
        id: fadeAnimator
        running: false

        OpacityAnimator {
            target: self
            from: initialOpacity
            to: 0
            duration: 1000
            running: false
        }
        ScriptAction { script: close() }
    }


    Timer {
        id: closingTimer
        interval: showingTime
        onTriggered: fadeAnimator.running = true
    }

    // TODO: first only single download, then maybe queue or paraller

    function onDownloadStarted(applicationFullId) {
        message = qsTr("Downloading ") + applicationFullId + " ..."
    }

    function onDownloadAborted(applicationFullId) {
        message = qsTr("Downloading of ") + applicationFullId + qsTr(" aborted.")
        closingTimer.running = true
    }

    function onDownloadFinished(applicationFullId) {
        message = qsTr("Downloading of ") + applicationFullId + qsTr(" finished.")
        closingTimer.running = true
    }

    function onDownloadProgress(applicationFullId, progressPercentage) {
        message = qsTr("Downloading ") + applicationFullId + " ..." + progressPercentage
    }

    Component.onCompleted: {
        DownloadModel.downloadStarted.connect(onDownloadStarted)
        DownloadModel.downloadAborted.connect(onDownloadAborted)
        DownloadModel.downloadFinished.connect(onDownloadFinished)
        DownloadModel.downloadProgress.connect(onDownloadProgress) // applicationFullId, progressPercentage
    }
}
