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
 
 #include "downloadablegamesmodel.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#define LOG_AREA "GameModel"
#include <log/log.h>

namespace GBerry {

class DownloadableGamesModelPrivate
{
public:
    DownloadableGamesModelPrivate(IDownloadableGamesModelCommunication* comm_, DownloadableGamesModel* q_) :
        q(q_),
        comm(comm_),
        gamesReceived(false) {

        _signalConnection = QObject::connect(comm, &IDownloadableGamesModelCommunication::messageReceived,
                                             [this] (const QJsonObject& msg) {
            this->onMessageReceived(msg);
        });
    }
    ~DownloadableGamesModelPrivate() {
        QObject::disconnect(_signalConnection);
    }

    DownloadableGamesModel* q;
    QMetaObject::Connection _signalConnection;
    IDownloadableGamesModelCommunication* comm;
    bool gamesReceived;
    QMap<QString, QVariantMap> games;

    void requestGames() {
        // TODO: add to a message factory?
        QJsonObject json;
        json["command"]  = "QueryDownloadableApplications";
        comm->sendMessage(json);
    }

    void queryGamesUpdated() {
        // TODO: message with timstamp
        // TODO: this is not needed in the beginning
    }

    void onMessageReceived(const QJsonObject& msg) {
        // TODO: how and where validate json?

        // message is for us, that is for sure, but is it ok / failure

        if (msg["result"].toString() == "failure") {
            gamesReceived = false;
            emit q->gamesRequestFailed();
        }
        else {
            // TODO: validation of json ... currently on comms side ...
            /*
            json["id"] = app.id();
            json["version"] = app.meta()->version();
            json["application_id"] = app.meta()->applicationId();
            json["name"] = app.meta()->name();
            json["description"] = app.meta()->description();
            json["catalog_image"] = app.meta()->catalogImageFilePath();
            json["status"] = app.stateString();
            */

            if (msg.contains("applications") && msg["applications"].isArray()) {
                foreach(auto appJsonValue, msg["applications"].toArray()) {
                    QJsonObject appJson(appJsonValue.toObject());
                    QVariantMap app(appJson.toVariantMap());
                    games[appJson["id"].toString()] = app;
                }
            }
            // TODO: how to handle different searches (well now first only one global search)
            //       -- will search local (cached data) or do we execute is always against server?
            gamesReceived = true;
            emit q->gamesAvailable();
        }
    }

};


DownloadableGamesModel::DownloadableGamesModel(
        IDownloadableGamesModelCommunication* comm,
        QObject *parent) :
    QObject(parent),
    _d(new DownloadableGamesModelPrivate(comm, this))
{
    /*
    QVariantMap details;

    details["id"] = "tictactoe-0.5";
    details["name"] = "TicTacToe";
    details["description"] = "Game of 'x' and 'o'. Try to get three in a row!";
    _games["tictactoe"] = details;

    details["id"] = "react";
    details["name"] = "React!";
    details["description"] = "Who enters fastest sequence of numbers?";
    _games["react"] = details;

    details["id"] = "testgame";
    details["name"] = "Test Game";
    details["description"] = "Just a test game.";
    _games["testgame"] = details;
    */
}

DownloadableGamesModel::~DownloadableGamesModel()
{

}

bool DownloadableGamesModel::requestGames()
{
    if (_d->gamesReceived) {
        // TODO: send query if update is needed (timestamp)
        return true;
    }
    // send query for games
    _d->requestGames();
    return false;
}

QStringList DownloadableGamesModel::gameIds() const
{
    return _d->games.keys();
}


QVariantMap DownloadableGamesModel::game(QString gameId) const
{
    if (_d->games.contains(gameId)) {
        return _d->games[gameId];
    } else {
        // not found, return empty
        QVariantMap empty;
        return empty;
    }
}

} // eon

// TODO: downloabable will include
//   - newer versions if already downloaded games
//   - ?? also already downloaded games ?? -> at this point no need to filter, UI can show them as installed
