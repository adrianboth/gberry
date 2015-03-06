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

    void loginGuest(QString guestName);
    void openConsoleConnection(ConsoleDevice console);
    void closeConsoleConnection();
    bool isConsoleConnectionOpen() const;

    void sendMessage(QString message);

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
