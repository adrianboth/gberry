#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

class WebsocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketClient(QObject *parent = Q_NULLPTR);
    ~WebsocketClient();

    void open(const QUrl &url);
    void close();
    bool isConnected();
    void sendMessage(QString message);

signals:
    void closed();
    void connected();
    void messageReceived(QString message);

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool _isConnected;
};

#endif // WEBSOCKETCLIENT_H
