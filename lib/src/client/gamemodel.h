#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>


class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = 0);
    ~GameModel();

    Q_INVOKABLE QStringList localGameIds() const;

    Q_INVOKABLE QVariantMap game(QString gameId) const;


signals:

public slots:

private:
    QMap<QString, QVariantMap> _games;
};

#endif // GAMEMODEL_H
