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


signals:
    void localGamesAvailable();
    void localGamesUpdated();

public slots:
    void onGameDownloaded(QString applicationFullId);

private:
    const QScopedPointer<GameModelPrivate> _d;
};

#endif // GAMEMODEL_H
