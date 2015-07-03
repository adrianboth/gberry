#ifndef DOWNLOADABLEGAMESMODEL_H
#define DOWNLOADABLEGAMESMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QScopedPointer>
#include <QStringList>
#include <QVariantMap>

namespace GBerry {

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

class DownloadableGamesModelPrivate;

class DownloadableGamesModel : public QObject
{
    Q_OBJECT

public:
    explicit DownloadableGamesModel(
            IDownloadableGamesModelCommunication* comm,
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

public slots:

private:
    const QScopedPointer<DownloadableGamesModelPrivate> _d;
};

} // eon

#endif // DOWNLOADABLEGAMESMODEL_H
