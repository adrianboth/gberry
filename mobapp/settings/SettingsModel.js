.pragma library

// TODO: how js shared between components?
.import QtQuick.LocalStorage 2.0 as LocalStorageAPI

.import "../ApplicationSettingsJS.js" as ApplicationSettingsJS

.import GBerry 1.0 as GBerry
GBerry.Log.initLog("SettingsModel", ApplicationSettingsJS.logLevel)

var activeConsoleIndex

function consoleAddress() {
    GBerry.Log.debug("Current activeConsoleIndex: " + activeConsoleIndex)
    return consoles[activeConsoleIndex].address
}

function indexOfCurrent() {
    return activeConsoleIndex
}

function addConsole(consoleAddress) {
    if (consoleAddress.length === 0)
        return

    // look for similar
    for (var i = 0; i < consoles.length; i++) {
        if (consoles[i].text === consoleAddress) {
            // same found -> do not add
            return false
        }
    }
    GBerry.Log.debug("Adding console: " + consoleAddress)
    consoles.push({address: consoleAddress, last_used: 0})

    var db = _getDB()
    db.transaction(
        function(tx) {
            // TODO: we don't have specific name yet
            tx.executeSql('INSERT INTO Console VALUES(?, ?, ?)', [consoleAddress, consoleAddress, 0]);
        }
    )
}

function setActiveConsole(consoleAddress) {
    for (var i = 0; i < consoles.length; i++) {
        console.debug("COMPARE: " + consoles[i].address + " vs " + consoleAddress)
        if (consoles[i].address === consoleAddress) {
            var prevAddress = consoles[activeConsoleIndex].address

            var db = _getDB()
            db.transaction(
                function(tx) {
                    tx.executeSql('UPDATE Console SET last_used=? WHERE address=?', [ 0, prevAddress ]);
                    tx.executeSql('UPDATE Console SET last_used=? WHERE address=?', [ 1, consoleAddress ]);
                }
            )

            consoles[activeConsoleIndex].last_used = 0
            consoles[i].last_used = 1

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
            tx.executeSql('CREATE TABLE IF NOT EXISTS Console(name TEXT, address TEXT, last_used INTEGER)');
        }
    )
    var consolesList = []
    db.transaction(
        function(tx) {
            // Show all added greetings
            var rs = tx.executeSql('SELECT * FROM Console');
            var row
            for(var i = 0; i < rs.rows.length; i++) {
                row = rs.rows.item(i)
                console.debug("Reading console: " + row.address)
                consolesList.push({name: row.name, address: row.address, last_used: row.last_used})
            }
        }
    )


    if (consolesList.length == 0) {
        db.transaction(
            function(tx) {
                tx.executeSql('INSERT INTO Console VALUES(?, ?, ?)', [ 'localhost', 'localhost', 1 ]);
            }
        )
        consolesList.push({address: "localhost", last_used: 1})
    }

    activeConsoleIndex = 0 // default

    for (var i = 0; i < consolesList.length; i++) {
        if (consolesList[i].last_used === 1) {
            activeConsoleIndex = i
            break
        }
    }

    return consolesList
}

var consoles = _initialize()
