#ifndef PLAYERSESSIONMANAGER_H
#define PLAYERSESSIONMANAGER_H

#include <QObject>
#include <QMap>


class PlayerSession;


class PlayerSessionManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerSessionManager(QObject *parent = 0);
    ~PlayerSessionManager();

    virtual bool isPlayerNameReserved(QString& playerName) const;
    virtual void insertSession(PlayerSession& session);
    virtual void removeSession(int playerId);
    virtual PlayerSession sessionByToken(QString token);

signals:

public slots:

private:
    QMap<QString, PlayerSession> _sessions;
};

#endif // PLAYERSESSIONMANAGER_H
