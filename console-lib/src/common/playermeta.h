#ifndef PLAYERMETA_H
#define PLAYERMETA_H

#include <QJsonObject>

// TODO: make this qml compatible
class PlayerMeta
{
public:
    PlayerMeta();
    PlayerMeta(int pid, QString name);
    PlayerMeta(const QJsonObject& metaJson);
    PlayerMeta(const PlayerMeta& other);

    PlayerMeta& operator=(const PlayerMeta& other);

    QJsonObject json();

    int playerId() const;
    const QString& playerName() const;

private:
    int _pid;
    QString _name;
};


#endif // PLAYERMETA_H
