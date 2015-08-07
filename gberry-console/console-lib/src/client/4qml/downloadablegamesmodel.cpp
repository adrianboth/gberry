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

#include "activeplayermodel.h"

#define LOG_AREA "GameModel"
#include <log/log.h>

namespace GBerryApplication {

class GameData {
public:
    GameData(const QString& gameId, const QVariantMap& metaData) :
        _gameId(gameId),
        _metaData(metaData) {}

    inline const QString& gameId() { return _gameId; }
    inline const QVariantMap& metaData() { return _metaData; }

private:
    QString _gameId;
    QVariantMap _metaData;
};

// guest players see only 'free' games and thus they can be combined
// NOTE: now gamedata is not shared between players (they might have same games)
//       but that is not important at this point
class GameDataCache {
public:
    ~GameDataCache() {
        foreach(auto gameDataForPlayer, cache.values()) {
            foreach (auto gameData, gameDataForPlayer->values()) {
                delete gameData;
            }
            delete gameDataForPlayer;
        }
    }

    void add(int playerId, GameData* data) {
        int pid = effectivePlayerId(playerId);
        if (!cache.contains(pid)) {
            cache[pid] = new QMap<QString, GameData*>;
        }
        cache[pid]->insert(data->gameId(), data);
    }

    void clear(int playerId) {
        if (!cache.contains(playerId))
            return;

        foreach (auto gameData, cache[playerId]->values()) {
            delete gameData;
        }
        cache[playerId]->clear();
    }

    QMap<QString, GameData*>& get(int playerId) {
        int pid = effectivePlayerId(playerId);
        if (cache.contains(pid)) {
            return *(cache[pid]);
        }

        return __emptyMap;
    }

    bool hasGames(int playerId) const {
        return cache.contains(effectivePlayerId(playerId));
    }

private:
    int effectivePlayerId(int playerId) const {
        if (playerId < -1)
            return -1;
        return playerId;
    }

    QMap<int, QMap<QString, GameData*>* > cache;
    static QMap<QString, GameData*> __emptyMap;
};
QMap<QString, GameData*> GameDataCache::__emptyMap;



class DownloadableGamesModelPrivate
{
public:
    DownloadableGamesModelPrivate(
            IDownloadableGamesModelCommunication* comm_,
            ActivePlayerModel* activePlayerModel_,
            DownloadableGamesModel* q_) :
        q(q_),
        comm(comm_),
        activePlayerModel(activePlayerModel_) {

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
    ActivePlayerModel* activePlayerModel;
    GameDataCache gamesCache;

    void requestGames() {
        // TODO: add to a message factory?
        QJsonObject json;
        json["command"]  = "QueryDownloadableApplications";
        if (activePlayerModel->hasActivePlayer()) {
            json["player_id"] = activePlayerModel->activePlayerId();
        }
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

            int playerId = -1; // default guest = free games
            if (msg.contains("player_id")) {
                playerId = msg["player_id"].toInt();
            }

            // we assume that we get all apps (i.e. some might have been removed)
            // TODO: single update separately
            gamesCache.clear(playerId);

            if (msg.contains("applications") && msg["applications"].isArray()) {
                foreach(auto appJsonValue, msg["applications"].toArray()) {
                    QJsonObject appJson(appJsonValue.toObject());
                    QVariantMap app(appJson.toVariantMap());
                    QString gameId = appJson["id"].toString();
                    gamesCache.add(playerId, new GameData(gameId, app));
                }
            }
            // TODO: how to handle different searches (well now first only one global search)
            //       -- will search local (cached data) or do we execute is always against server?

            emit q->gamesAvailable();
        }
    }

};



DownloadableGamesModel::DownloadableGamesModel(
        IDownloadableGamesModelCommunication* comm,
        ActivePlayerModel* activePlayerModel,
        QObject *parent) :
    QObject(parent),
    _d(new DownloadableGamesModelPrivate(comm, activePlayerModel, this))
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
    if (_d->gamesCache.hasGames(_d->activePlayerModel->activePlayerId())) {

        // TODO: send query if update is needed (timestamp)
        // TODO: now sending always
        DEBUG("Old data returned, requesting update");
        _d->requestGames();
        return true;
    }

    // send query for games
    DEBUG("No data, requesting it");
    _d->requestGames();
    return false;
}

QStringList DownloadableGamesModel::gameIds() const
{
    if (_d->gamesCache.hasGames(_d->activePlayerModel->activePlayerId())) {
        return _d->gamesCache.get(_d->activePlayerModel->activePlayerId()).keys();
    }
    return QStringList();
}


QVariantMap DownloadableGamesModel::game(QString gameId) const
{
    int activePlayerId = _d->activePlayerModel->activePlayerId();

    if (!_d->gamesCache.hasGames(activePlayerId)) {
        return QVariantMap();
    }

    QMap<QString, GameData*> data = _d->gamesCache.get(activePlayerId);

    if (data.contains(gameId)) {
        return data[gameId]->metaData();

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
