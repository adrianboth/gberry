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

signals:
    void received(const QByteArray& msg);

public slots:
    void socketReadyRead();

private:
    QTcpSocket* _socket;
    int _blockSize;
    QByteArray _data;
};

#endif // MESSAGEREADER_H
