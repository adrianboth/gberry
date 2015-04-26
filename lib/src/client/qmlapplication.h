#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

#include "consolesessionmanager.h"
#include "consoledevice.h"


namespace mobile
{

/**
 * @brief Application abstraction for QML side
 *
 * Exposes properties and function to root QML context.
 */
class QmlApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)

public:
    explicit QmlApplication(QObject *parent = 0);
    ~QmlApplication();

    Q_INVOKABLE void loginGuest(QString guestName);
    Q_INVOKABLE void openConsoleConnection(ConsoleDevice console);
    Q_INVOKABLE void openConsoleConnection(QString hostName);
    Q_INVOKABLE void closeConsoleConnection();
    Q_INVOKABLE bool isConsoleConnectionOpen() const;

    Q_INVOKABLE void sendMessage(QString message);

    bool loggedIn() const { return _loggedIn; }

signals:
    void consoleConnectionOpened();
    void consoleConnectionClosed();
    void consoleConnectionOpenFailed(QString errorMsg);
    void playerMessageReceived(QString message);

    void loggedInChanged();

public slots:
    void onConsoleSessionOpened();
    void onConsoleSessionOpenFailed(QString message);
    void onWebsocketMessageReceived(QString message);
    void onWebsocketClosed();

private:
    ConsoleSessionManager _consoleSession;
    QString _currentPlayerName;
    bool _loggedIn; // using separate var, as isConsoleConnectionOpen() is about websocket
};

} // eon

#endif // APPLICATION_H
