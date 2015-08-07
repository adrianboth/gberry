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
 
 #include "gamemodel.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#define LOG_AREA "GameModel"
#include <log/log.h>

#include "client/applicationversionnumber.h"
using namespace GBerryLib;

class GameModelPrivate
{
public:
    GameModelPrivate(IGameModelCommunication* comm_, GameModel* q_) :
        q(q_),
        comm(comm_),
        gamesReceived(false) {

        _signalConnection = QObject::connect(comm, &IGameModelCommunication::messageReceived,
                                             [this] (const QJsonObject& msg) {
            this->onMessageReceived(msg);
        });
    }
    ~GameModelPrivate() {
        QObject::disconnect(_signalConnection);
    }

    GameModel* q;
    QMetaObject::Connection _signalConnection;
    IGameModelCommunication* comm;
    bool gamesReceived;
    QMap<QString, QVariantMap> games;

    void requestLocalGames() {
        // TODO: add to a message factory?
        QJsonObject json;
        json["command"]  = "QueryLocalApplications";
        comm->sendMessage(json);
    }

    void requestLocalGame(const QString& applicationFullId) {
        // TODO: add to a message factory?
        QJsonObject json;
        json["command"]  = "QueryLocalApplications";
        json["application_id"] = applicationFullId;
        comm->sendMessage(json);
    }

    void queryGamesUpdated() {
        // TODO: message with timstamp
        // TODO: this is not needed in the beginning
    }

    void onMessageReceived(const QJsonObject& msg) {
        // handling is same, now matter if all or single game
        // was requested.

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
        bool oldGamesReceived = gamesReceived;
        gamesReceived = true;

        emit q->localGamesAvailable();

        // if there was previously fetched games -> inform that we have update
        if (oldGamesReceived) {
            emit q->localGamesUpdated();
        }
    }

};


GameModel::GameModel(IGameModelCommunication* comm, QObject *parent) :
    QObject(parent),
    _d(new GameModelPrivate(comm, this))
{
    // TODO: how localization?

    // TODO: using test data
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

GameModel::~GameModel()
{

}

bool GameModel::requestLocalGames()
{
    if (_d->gamesReceived) {
        // TODO: send query if update is needed (timestamp)
        return true;
    }
    // send query for games
    _d->requestLocalGames();
    return false;
}

QStringList GameModel::localGameIds() const
{
    QMap<QString, QVariantMap> gameById; // not full id
    foreach(QVariantMap gameMeta, _d->games.values()) {
        QString appId = gameMeta["application_id"].toString();
        QString version = gameMeta["version"].toString();

        if (gameById.contains(appId)) {
            QString otherVersion = gameById[appId]["version"].toString();
            if (ApplicationVersionNumber::versionIsGreater(version, otherVersion))
                gameById[appId] = gameMeta;

        } else {
            gameById[appId] = gameMeta;
        }
    }

    // we have applicationIds, not full ids
    QStringList fullIds;
    foreach(QVariantMap gameMeta, gameById.values()) {
        fullIds.append(gameMeta["id"].toString());
    }

    return fullIds;
}


QVariantMap GameModel::game(QString gameId) const
{
    if (_d->games.contains(gameId)) {
        return _d->games[gameId];
    } else {
        // not found, return empty
        QVariantMap empty;
        return empty;
    }
}

QString GameModel::newestGameByApplicationId(const QString& applicationId) const
{
    QMap<QString, QVariantMap> gameById; // not full id
    foreach(QVariantMap gameMeta, _d->games.values()) {
        QString appId = gameMeta["application_id"].toString();
        QString version = gameMeta["version"].toString();

        if (gameById.contains(appId)) {
            QString otherVersion = gameById[appId]["version"].toString();
            if (ApplicationVersionNumber::versionIsGreater(version, otherVersion))
                gameById[appId] = gameMeta;

        } else {
            gameById[appId] = gameMeta;
        }
    }

    if (gameById.contains(applicationId))
        return gameById[applicationId]["id"].toString();

    return QString();
}

void GameModel::onGameDownloaded(QString applicationFullId)
{
    if (_d->gamesReceived) {
        _d->requestLocalGame(applicationFullId);
    }
}
