#ifndef MESSAGEREADER_H
#define MESSAGEREADER_H

#include <QObject>
#include <QByteArray>

class QTcpSocket;


/**
 * MessageReader reads prefixed length messaged from TCP stream.
 */
class MessageReader : public QObject
{
    Q_OBJECT
public:
    explicit MessageReader(QTcpSocket* socket, QObject *parent = 0);
    ~MessageReader();

    void write(int channelId, const QByteArray msg);

signals:
    void received(int channelId, const QByteArray msg);

public slots:
    void socketReadyRead();

private:
    QTcpSocket* _socket;
    quint32 _blockSize;
    quint32 _channelId;
    QByteArray _data;
};

#endif // MESSAGEREADER_H
