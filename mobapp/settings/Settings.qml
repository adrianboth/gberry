import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.LocalStorage 2.0

Rectangle {
    anchors.fill: parent
    //color: "blue"

    function consoleAddress() {
        return consoleAddress.editText
    }

    function serverAddress() {
        return consoleAddress.editText
    }

    ScrollView {
        id: page
        anchors.fill: parent

        ColumnLayout {
            Text { text: "Settings"; font.pointSize: 16 }
            width: mainwindow.width
            //anchors.left: parent.left
            //anchors.right: parent.right
            //anchors.fill: parent
/*
            Rectangle {
                color: "red"
                anchors.left: parent.left
                anchors.right: parent.right
                height: 50
            }
*/
            GroupBox {
                title: "Console"
                //Layout.fillWidth: true
                anchors.left: parent.left
                anchors.right: parent.right

                RowLayout {
                    anchors.fill: parent
                    Label { text: "Console address"; }
                    ComboBox {
                        id: consoleAddress
                        editable: true
                        Layout.fillWidth: true
                        model: consoleModel

                        onAccepted: {
                             if (find(currentText) === -1) {
                                 model.append({text: editText})
                                 currentIndex = find(editText)
                             }
                         }
                    }
                }

                ListModel {
                    id: consoleModel
                    ListElement { text: "localhost" }
                }
            }

            GroupBox {
                title: "Server"
                width: mainwindow.width
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
                    Label { text: "Server address"; }
                    ComboBox {
                        id: serverAddress
                        editable: true
                        Layout.fillWidth: true
                        model: serverModel

                        onAccepted: {
                             if (find(currentText) === -1) {
                                 model.append({text: editText})
                                 currentIndex = find(editText)

                                 var db = LocalStorage.openDatabaseSync("QQmlExampleDB", "1.0", "The Example QML SQL!", 1000000);
                                 db.transaction(
                                     function(tx) {
                                         tx.executeSql('INSERT INTO ServerAddress VALUES(?)', [ editText ]);
                                     }
                                 )
                             }
                         }
                    }
                }

                ListModel {
                    id: serverModel
                    //ListElement { text: "localhost" }
                }
            }
/*
            Rectangle {
                id: test
                color: "red"
                height: 50
                width: mainwindow.width
            }
*/

            Text {
                id: dbTest
                text: "?"
                anchors.horizontalCenter: parent.horizontalCenter

            }
        }
    }

    Component.onCompleted: {
        var db = LocalStorage.openDatabaseSync("QQmlExampleDB", "1.0", "The Example QML SQL!", 1000000);

        db.transaction(
            function(tx) {
                // Create the database if it doesn't already exist
                tx.executeSql('CREATE TABLE IF NOT EXISTS ServerAddress(address TEXT)');
                // Add (another) greeting row
                tx.executeSql('INSERT INTO ServerAddress VALUES(?)', [ 'localhost' ]);
            }
        )

        db = LocalStorage.openDatabaseSync("QQmlExampleDB", "1.0", "The Example QML SQL!", 1000000);

        db.transaction(
            function(tx) {
                // Show all added greetings
                var rs = tx.executeSql('SELECT * FROM ServerAddress');

                for(var i = 0; i < rs.rows.length; i++) {
                    serverModel.append({text: rs.rows.item(i).address})
                }
            }
        )

    }
}

