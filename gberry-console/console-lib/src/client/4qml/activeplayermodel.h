#ifndef ACTIVEPLAYERMODEL_H
#define ACTIVEPLAYERMODEL_H

#include <QObject>
#include <QScopedPointer>

namespace GBerryApplication {

class ActivePlayerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int activePlayerId READ activePlayerId NOTIFY activePlayerIdChanged)
    Q_PROPERTY(bool hasActivePlayer READ hasActivePlayer NOTIFY hasActivePlayerChanged)
    Q_PROPERTY(bool activePlayerIsGuest READ activePlayerIsGuest NOTIFY activePlayerIsGuestChanged)

public:
    ActivePlayerModel();
    ~ActivePlayerModel();

    Q_INVOKABLE void activatePlayer(int playerId);
    Q_INVOKABLE void deactivatePlayer();
    Q_INVOKABLE bool isActivePlayerDevelopmentUser();

    // -- properties
    bool hasActivePlayer() const;
    int activePlayerId() const;
    bool activePlayerIsGuest() const;

signals:
    void activePlayerIdChanged();
    void hasActivePlayerChanged();
    void activePlayerIsGuestChanged();

private:
    class Private;
    const QScopedPointer<Private> _d;
};

} // eon

#endif // ACTIVEPLAYERMODEL_H
