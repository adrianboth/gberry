#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

#include "consolesessionmanager.h"
#include "consoledevice.h"


namespace mobile
{

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);
    ~Application();

    Q_INVOKABLE void loginGuest(QString guestName);
    Q_INVOKABLE void openConsoleConnection(ConsoleDevice console);
    Q_INVOKABLE void openConsoleConnection(QString hostName);
    Q_INVOKABLE void closeConsoleConnection();
    Q_INVOKABLE bool isConsoleConnectionOpen() const;

    Q_INVOKABLE void sendMessage(QString message);

signals:
    void consoleConnectionOpened();
    void consoleConnectionClosed();
    void playerMessageReceived(QString message);

public slots:
    void onConsoleSessionOpened();
    void onWebsocketMessageReceived(QString message);
    void onWebsocketClosed();

private:
    ConsoleSessionManager _consoleSession;
    QString _currentPlayerName;
};

} // eon

#endif // APPLICATION_H
