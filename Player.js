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
 
 //.pragma library

/* Player object */
function Player(pid, pname) {
    this.id = pid
    this.name = pname

    this.isValid = function() {
        return true
    }
}

function UndefinedPlayer() {
    this.id = 0
    this.name = "<undefined>"

    this.isValid = function() {
        return false
    }
}

/* Composition: Players of the game */
function Players(__player1, __player2) {
    this.player1 = __player1
    this.player2 = __player2
}
