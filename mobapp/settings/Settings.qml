import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.LocalStorage 2.0

import "SettingsModel.js" as SettingsModel

Rectangle {
    anchors.fill: parent
    //color: "blue"

    function consoleAddress() {
        return consoleAddress.editText
    }

    function serverAddress() {
        return serverAddress.editText
    }

    onVisibleChanged: {
        // when hidden make sure we do closing actions
        if (!visible)
            close()
        else {
            consoleAddress.currentIndex = SettingsModel.indexOfCurrentConsole()
            serverAddress.currentIndex = SettingsModel.indexOfCurrentServer()
        }
    }

    // should be called when this view is closed externally
    function close() {
        // it might be that settings were edited but not accepted
        // (especially on desktop devenv when you need to press Enter to 'accept'

        //console.debug("CLOSE CALLED: " + consoleAddress.currentText)
        //console.debug("INDEX: " + consoleAddress.currentIndex)
        //console.debug("EDIT TEXT: " + consoleAddress.editText)

        // if address already listed - doesn't do anything
        SettingsModel.addConsole(consoleAddress.editText)
        SettingsModel.setActiveConsole(consoleAddress.editText)

        SettingsModel.addServer(serverAddress.editText)
        SettingsModel.setActiveServer(serverAddress.editText)
    }

    ScrollView {
        id: page
        anchors.fill: parent

        ColumnLayout {
            width: root.width

            Text { text: "Settings"; font.pointSize: 16 }

            GroupBox {
                title: "Console"
                //Layout.fillWidth: true
                anchors.left: parent.left
                anchors.right: parent.right

                RowLayout {
                    anchors.fill: parent
                    Label { text: "Console address" }
                    ComboBox {
                        id: consoleAddress
                        editable: true
                        Layout.fillWidth: true
                        model: consoleModel
                        onActivated: {
                            console.debug("Set active console to " + textAt(index))
                            SettingsModel.setActiveConsole(textAt(index))
                        }

                        onAccepted: {
                             if (find(currentText) === -1) {
                                 SettingsModel.addConsole(editText)
                                 SettingsModel.setActiveConsole(editText)
                                 refreshConsoleListModel()
                                 currentIndex = find(editText)
                             }
                         }
                    }
                }

                ListModel {
                    id: consoleModel
                    //ListElement { text: "localhost" }
                }
            }

            GroupBox {
                title: "Server"
                width: root.width
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    Label { text: "Server address"; }
                    ComboBox {
                        id: serverAddress
                        editable: true
                        Layout.fillWidth: true
                        model: serverModel

                        onActivated: {
                            console.debug("Set active server to " + textAt(index))
                            SettingsModel.setActiveServer(textAt(index))
                        }

                        onAccepted: {
                             if (find(currentText) === -1) {
                                 SettingsModel.addServer(editText)
                                 SettingsModel.setActiveServer(editText)
                                 refreshServerListModel()
                                 currentIndex = find(editText)
                             }
                        }

                    }
                }

                ListModel {
                    id: serverModel
                    //ListElement { text: "localhost" }
                }
            }

        }
    }

    function refreshConsoleListModel() {
        var consoles = SettingsModel.consoles
        consoleModel.clear() // remove all previous
        for (var i = 0; i < consoles.length; i++) {
            consoleModel.append({text: consoles[i].address})
        }
    }

    function refreshServerListModel() {
        var servers = SettingsModel.servers
        serverModel.clear() // remove all previous
        for (var i = 0; i < servers.length; i++) {
            serverModel.append({text: servers[i].address})
        }
    }

    Component.onCompleted: {
        refreshConsoleListModel()
        refreshServerListModel()
    }
}
