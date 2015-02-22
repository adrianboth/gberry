#include "messagereader.h"

#include <QTcpSocket>
#include <QDataStream>

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
        // we need at needs 4 bytes to present length
        if (_socket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> _blockSize;
    }

    if (_socket->bytesAvailable() < _blockSize)
        return;

    _data.clear();
    in >> _data;

    // TODO: what if there is second message ready to be read? I guess we don't other readyRead() signal
    emit received(_data);
}


void MessageReader::write(const QByteArray &msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    out << (quint32)0;
    out << msg;
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));

    _socket->write(block);
}
