#ifndef CONNECTIONGATEKEEPER_H
#define CONNECTIONGATEKEEPER_H

#include <QObject>
#include <QScopedPointer>

namespace GBerry {
namespace Console {
namespace Server {

class ConnectionGateKeeperPrivate;
class ApplicationRegistry;

/**
 * @brief The ConnectionGateKeeper class
 *
 * First a ping response and possible identification code is required from a
 * new connection.
 */
class ConnectionGateKeeper : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionGateKeeper(int connectionId, ApplicationRegistry* appRegistry, QObject *parent = 0);
    ~ConnectionGateKeeper();

    void validate();
    void incomingMessage(const QByteArray& msg);
    QString validatedApplicationId();

signals:
    void connectionValidated(int connectionId);
    void connectionDiscarded(int connectionId);

    void outgoingMessage(int connectionId, const QByteArray msg);

public slots:
    void waitTimeout();

private:
    const QScopedPointer<ConnectionGateKeeperPrivate> _d;
};

}}} // eon

#endif // CONNECTIONGATEKEEPER_H
