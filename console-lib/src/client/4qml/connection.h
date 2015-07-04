#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QScopedPointer>

class ClientSideControlChannel;
class ConnectionPrivate;

/**
 * @brief Connection information for 4qml
 *
 * This class provides connection information towards qml.
 *   - Connected: If validated TCP connection to 'comms' exists
 *   - Activated: If connected players communicate with our application
 *
 * During life cycle of an app typically both 'connected' and 'activated'
 * change from 'false' to 'true' in during application startup. The
 * application should wait 'connected=true' before sending any commands
 * to 'comms'.
 *
 * For normal applications if these values change back to false it means
 * that that application should be about to close, or it can close itself.
 *
 * However during development time of GBerry platform it is usefull if
 * applications can recover from 'connection going down' i.e. 'comms' is
 * restarted.
 *
 * HeadServerConnection means whether GBerry platform has a connection to
 * GBerry server in Internet. If not then for example querying downloadable
 * applications and downloading them is not possible.
 */
class Connection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(bool isActivated READ isActivated NOTIFY isActivatedChanged)
    Q_PROPERTY(bool isHeadServerConnected READ isHeadServerConnected NOTIFY isHeadServerConnectedChanged)

public:
    explicit Connection(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    ~Connection();

    bool isConnected() const;
    bool isActivated() const;
    bool isHeadServerConnected() const;

signals:
    void isConnectedChanged();
    void isActivatedChanged();
    void isHeadServerConnectedChanged();

public slots:
    void onActivatedChanged();
    void onConnectedChanged();
    void onHeadServerConnectedChanged();

private:
    const QScopedPointer<ConnectionPrivate> _d;

};

#endif // CONNECTION_H
