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
 
 #ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QStringList>
#include <QVariantMap>
#include <QScopedPointer>


class IGameModelCommunication : public QObject
{
    Q_OBJECT
public:
    explicit IGameModelCommunication(QObject *parent = 0) : QObject(parent) {}
    virtual ~IGameModelCommunication() {}

    virtual void sendMessage(const QJsonObject& msg) = 0;

signals:
    void messageReceived(const QJsonObject msg);
};


class GameModelPrivate;

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(IGameModelCommunication* comm, QObject *parent = 0);
    ~GameModel();

    // returns true if games are available
    Q_INVOKABLE bool requestLocalGames();

    Q_INVOKABLE QStringList localGameIds() const;

    Q_INVOKABLE QVariantMap game(QString gameId) const;

    // returns empty string if no apps by specified applicationId
    Q_INVOKABLE QString newestGameByApplicationId(const QString& applicationId) const;


signals:
    void localGamesAvailable();
    void localGamesUpdated();

public slots:
    void onGameDownloaded(QString applicationFullId);

private:
    const QScopedPointer<GameModelPrivate> _d;
};

#endif // GAMEMODEL_H
