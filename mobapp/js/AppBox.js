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
 
 var appboxObject

var _parentItem
var _emptyContentObject
var _outgoingMessageFuncRef

function initialiaze(parentItem) {
    _parentItem = parentItem

    var emptyContent = 'import QtQuick 2.0;\
Rectangle {\
color: "lightgrey";\
border.width: 1;\
border.color: "black";\
width: 200;\
height: 50;\
anchors.centerIn: parent;\
Text {\
  text: "No content";\
  anchors.centerIn: parent;\
}\
signal outgoingMessage(var data);\
function incomingMessage(data) {\n\
console.debug("### APPBOX MSG: " + data)\n\
outgoingMessage("got appbox msg")\n\
}\n\
}'
    _emptyContentObject = Qt.createQmlObject(emptyContent, parentItem, "emptyAppBoxContent");
    _emptyContentObject.visible = false
}

function createContent(qmlContent) {
    releaseContent() // possible previous content
    console.debug("### AppBox.createContent(): " + qmlContent)
    console.debug("### typeof " + typeof(qmlContent))
    appboxObject = Qt.createQmlObject(qmlContent, _parentItem, "dynamicAppBoxContent");

    appboxObject.visible = false

    // (re)create registered listeners
    if (typeof(_outgoingMessageFuncRef) != 'undefined') {
        appboxObject.outgoingMessage.connect(_outgoingMessageFuncRef)
    }
}


function showContent() {
    if (typeof(appboxObject) != 'undefined') {
        appboxObject.visible = true
        _emptyContentObject.visible = false
    } else {
        _emptyContentObject.visible = true
    }
}

// just visibility -> false
function hideContent() {
    if (typeof(appboxObject) != 'undefined') {
        appboxObject.visible = false
        _emptyContentObject.visible = false
    }
}

function releaseContent() {

    if (typeof(appboxObject) != 'undefined') {
        var undef
        appboxObject.visible = false
        appboxObject.destroy()
        appboxObject = undef
        // we are not changing visibility of empty content
    }
}

function sendMessage(msg) {
    if (typeof(appboxObject) != 'undefined') {
        appboxObject.incomingMessage(msg)
    }
}

function connectOutgoingMessageTo(funcRef) {
    // only one function can be active
    _outgoingMessageFuncRef = funcRef
    if (typeof(appboxObject) != 'undefined') {
        appboxObject.outgoingMessage.connect(_outgoingMessageFuncRef)
    }
}
