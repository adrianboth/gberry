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

    emit received(_data);
}
