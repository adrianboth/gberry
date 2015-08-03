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
 
 .pragma library

// TODO: how js shared between components?
.import QtQuick.LocalStorage 2.0 as LocalStorageAPI

.import "../ApplicationSettingsJS.js" as ApplicationSettingsJS

.import GBerry 1.0 as GBerry
GBerry.Log.initLog("SettingsModel", ApplicationSettingsJS.logLevel)

var activeConsoleIndex
var activeServerIndex
var consoles
var servers
var serverConnectionModel // C++ model not visible to JS lib

function consoleAddress() {
    GBerry.Log.debug("Current activeConsoleIndex: " + activeConsoleIndex)
    return consoles[activeConsoleIndex].address
}

function serverAddress() {
    GBerry.Log.debug("Current activeServerIndex: " + activeServerIndex)
    return servers[activeServerIndex].address
}

function indexOfCurrentConsole() {
    return activeConsoleIndex
}

function indexOfCurrentServer() {
    return activeServerIndex
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

function addServer(serverAddress) {
    if (serverAddress.length === 0)
        return

    // look for similar
    for (var i = 0; i < servers.length; i++) {
        if (servers[i].text === serverAddress) {
            // same found -> do not add
            return false
        }
    }
    GBerry.Log.debug("Adding server: " + serverAddress)
    servers.push({address: serverAddress, last_used: 0})

    var db = _getDB()
    db.transaction(
        function(tx) {
            // TODO: we don't have specific name yet
            tx.executeSql('INSERT INTO Server VALUES(?, ?, ?)', [serverAddress, serverAddress, 0]);
        }
    )
}


function setActiveConsole(consoleAddress) {
    for (var i = 0; i < consoles.length; i++) {
        //console.debug("COMPARE: " + consoles[i].address + " vs " + consoleAddress)
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

            //console.debug("activeConsoleIndex=" + i)
            activeConsoleIndex = i
        }
    }
}

function setActiveServer(serverAddress) {
    for (var i = 0; i < servers.length; i++) {
        //console.debug("COMPARE: " + servers[i].address + " vs " + serverAddress)
        if (servers[i].address === serverAddress) {
            var prevAddress = servers[activeServerIndex].address

            var db = _getDB()
            db.transaction(
                function(tx) {
                    tx.executeSql('UPDATE Server SET last_used=? WHERE address=?', [ 0, prevAddress ]);
                    tx.executeSql('UPDATE Server SET last_used=? WHERE address=?', [ 1, serverAddress ]);
                }
            )

            servers[activeServerIndex].last_used = 0
            servers[i].last_used = 1

            //console.debug("activeServerIndex=" + i)
            activeServerIndex = i

            serverConnectionModel.setServerHost(serverAddress)
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
            tx.executeSql('CREATE TABLE IF NOT EXISTS Server(name TEXT, address TEXT, last_used INTEGER)');
        }
    )
    var consolesList = []
    db.transaction(
        function(tx) {
            var rs = tx.executeSql('SELECT * FROM Console');
            var row
            for(var i = 0; i < rs.rows.length; i++) {
                row = rs.rows.item(i)
                //console.debug("Reading console: " + row.address)
                consolesList.push({name: row.name, address: row.address, last_used: row.last_used})
            }
        }
    )

    var serversList = []
    db.transaction(
        function(tx) {
            var rs = tx.executeSql('SELECT * FROM Server');
            var row
            for(var i = 0; i < rs.rows.length; i++) {
                row = rs.rows.item(i)
                //console.debug("Reading server: " + row.address)
                serversList.push({name: row.name, address: row.address, last_used: row.last_used})
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

    if (serversList.length == 0) {
        db.transaction(
            function(tx) {
                tx.executeSql('INSERT INTO Server VALUES(?, ?, ?)', [ 'localhost', 'localhost', 1 ]);
            }
        )
        serversList.push({address: "localhost", last_used: 1})
    }

    activeConsoleIndex = 0 // default
    activeServerIndex = 0 // default

    for (var i = 0; i < consolesList.length; i++) {
        if (consolesList[i].last_used === 1) {
            activeConsoleIndex = i
            break
        }
    }

    for (var i = 0; i < serversList.length; i++) {
        if (serversList[i].last_used === 1) {
            activeServerIndex = i
            break
        }
    }

    // update global vars
    consoles = consolesList
    servers = serversList
}

_initialize()

// read from DB
function init(serverConnectionModel_) {
    serverConnectionModel = serverConnectionModel_
    serverConnectionModel.setServerHost(serverAddress())
}
