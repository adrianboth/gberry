.pragma library

// TODO: how js shared between components?
.import QtQuick.LocalStorage 2.0 as LocalStorageAPI

.import "../ApplicationSettingsJS.js" as ApplicationSettingsJS

.import GBerry 1.0 as GBerry
GBerry.Log.initLog("SettingsModel", ApplicationSettingsJS.logLevel)

//var consoles = []
var activeConsoleIndex

function consoleAddress() {
    GBerry.Log.debug("Current activeConsoleIndex: " + activeConsoleIndex)
    return consoles[activeConsoleIndex].text
}

function addConsole(consoleAddress) {
    // look for similar
    for (var i = 0; i < consoles.length; i++) {
        if (consoles[i].text === consoleAddress) {
            // same found -> do not add
            return false
        }
    }
    GBerry.Log.debug("Adding console: " + consoleAddress)
    consoles.push({text: consoleAddress})

    var db = _getDB()
    db.transaction(
        function(tx) {
            tx.executeSql('INSERT INTO ConsoleAddress VALUES(?)', [ consoleAddress]);
        }
    )
}

function setActiveConsole(consoleAddress) {
    for (var i = 0; i < consoles.length; i++) {
        console.debug("COMPARE: " + consoles[i].text + " vs " + consoleAddress)
        if (consoles[i].text === consoleAddress) {
            console.debug("activeConsoleIndex=" + i)
            activeConsoleIndex = i
        }
    }
}

function _getDB() {
    return LocalStorageAPI.LocalStorage.openDatabaseSync("GBerryMobAppDB", "1.0", "The Example QML SQL!", 1000000);
}

function _initialize() {
    var db = _getDB()
    db.transaction(
        function(tx) {
            // Create the database if it doesn't already exist
            tx.executeSql('CREATE TABLE IF NOT EXISTS ConsoleAddress(address TEXT)');
            // Add (another) greeting row
            //tx.executeSql('INSERT INTO ServerAddress VALUES(?)', [ 'localhost' ]);
        }
    )
    var consolesList = []
    db.transaction(
        function(tx) {
            // Show all added greetings
            var rs = tx.executeSql('SELECT * FROM ConsoleAddress');

            for(var i = 0; i < rs.rows.length; i++) {
                consolesList.push({text: rs.rows.item(i).address})
            }
        }
    )


    if (consolesList.length == 0) {
        db.transaction(
            function(tx) {
                tx.executeSql('INSERT INTO ConsoleAddress VALUES(?)', [ 'localhost' ]);
            }
        )
        consolesList.push({text: "localhost"})
    }

    // TODO: we should store last selection -> now always first
    activeConsoleIndex = 0
    return consolesList
}

var consoles = _initialize()
