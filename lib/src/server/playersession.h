#ifndef PLAYERSESSION_H
#define PLAYERSESSION_H

#include <QObject>
#include <QString>


class PlayerSession : public QObject
{
    Q_OBJECT

public:
    PlayerSession();
    explicit PlayerSession(int playerId, QString playerName, QString token);
    PlayerSession(const PlayerSession& other);
    ~PlayerSession();

    PlayerSession& operator =(const PlayerSession& other);

    int playerId() { return _id; }
    QString playerName() { return _name; }
    QString token() { return _token; }

    bool isValid() { return _token.length() > 0; }

    // TODO: who generates tokens

    // TODO:guest players

signals:

public slots:

private:
    int _id;
    QString _name;
    QString _token;
};


class InvalidPlayerSession : public PlayerSession
{
public:
    InvalidPlayerSession() : PlayerSession(-1, "INVALID", "") {}

};

class GuestPlayerSession : public PlayerSession
{
public:
    GuestPlayerSession(QString guestName, QString token) :
        PlayerSession(-2, guestName, token) {}
};

#endif // PLAYERSESSION_H
