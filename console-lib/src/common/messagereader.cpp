#include "messagereader.h"

#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>


MessageReader::MessageReader(QTcpSocket* socket, QObject *parent)
    : QObject(parent), _socket(socket), _blockSize(0)
{
    connect(_socket, &QTcpSocket::readyRead,
            this,    &MessageReader::socketReadyRead);
}

MessageReader::~MessageReader()
{
    disconnect(_socket, &QTcpSocket::readyRead,
               this,    &MessageReader::socketReadyRead);
}

void MessageReader::socketReadyRead()
{
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_4);

    if (_blockSize == 0)
    {
        // we need at needs 4+4 bytes to present length + channel
        if (_socket->bytesAvailable() < (int)sizeof(quint32)*2)
            return;

        in >> _blockSize;
        in >> _channelId;
    }
    // 4+4 is not calculated to message length
    if (_socket->bytesAvailable() < _blockSize)
        return;

    _data.clear();
    in >> _data;
    //qDebug() << "### MessageReader: cid=" << _channelId << QString(_data);
    emit received(_channelId, _data);

    _blockSize = 0;
    _channelId = 0;

    // If there is more to read
    if (_socket->bytesAvailable() > 0)
        socketReadyRead();
}


void MessageReader::write(int channelId, const QByteArray msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    out << (quint32)0;
    out << channelId;
    out.writeBytes(msg, msg.length());
    //out << msg;
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32)*2);

    _socket->write(block);
    //qDebug() << "### MessageReader wrote: " << QString(msg);
}
