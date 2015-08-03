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
 
 #ifndef DOWNLOADABLEGAMESMODEL_H
#define DOWNLOADABLEGAMESMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QScopedPointer>
#include <QStringList>
#include <QVariantMap>

namespace GBerryApplication {

class IDownloadableGamesModelCommunication : public QObject
{
    Q_OBJECT
public:
    explicit IDownloadableGamesModelCommunication(QObject *parent = 0) : QObject(parent) {}
    virtual ~IDownloadableGamesModelCommunication() {}

    virtual void sendMessage(const QJsonObject& msg) = 0;

signals:
    void messageReceived(const QJsonObject msg);
};


// TODO: in first version not thinking about split search results, there are so few games

class ActivePlayerModel;

class DownloadableGamesModelPrivate;

class DownloadableGamesModel : public QObject
{
    Q_OBJECT

public:
    explicit DownloadableGamesModel(
            IDownloadableGamesModelCommunication* comm,
            ActivePlayerModel* activePlayerModel,
            QObject* parent = 0);

    ~DownloadableGamesModel();

    // returns true if games are available
    Q_INVOKABLE bool requestGames();

    Q_INVOKABLE QStringList gameIds() const;

    Q_INVOKABLE QVariantMap game(QString gameId) const;

signals:
    void gamesAvailable();
    void gamesUpdated();

    // because search happens against server in internet there can be failures
    // like 'no connection'
    void gamesRequestFailed(); // TODO: should have some kind of error message / code?

private:
    const QScopedPointer<DownloadableGamesModelPrivate> _d;
};

} // eon

#endif // DOWNLOADABLEGAMESMODEL_H
