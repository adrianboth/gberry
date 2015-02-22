#include "commtcpclient.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

CommTcpClient::CommTcpClient(int port, QObject *parent) :
    QObject(parent), _port(port), _socket(NULL)
{

}

CommTcpClient::~CommTcpClient()
{
    if (_socket)
    {

        disconnect(_socket, &QTcpSocket::readyRead,
                this,       &CommTcpClient::dataReady);

        disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,       SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function

        disconnect(_socket, &QTcpSocket::disconnected,
                this,    &CommTcpClient::sockecDisconnected); // TODO: spelling mistake

        connect(_socket, &QTcpSocket::disconnected,
                _socket, &QTcpSocket::deleteLater);

        _socket->abort();
        //_socket->deleteLater();
    }
}

void CommTcpClient::sockecDisconnected()
{
    // someone closed to socket -> reconnect
    open();
}

void CommTcpClient::open()
{
    if (_socket)
    {
        disconnect(_socket, &QTcpSocket::readyRead,
                this,    &CommTcpClient::dataReady);

        disconnect(_socket, &QTcpSocket::disconnected,
                this,    &CommTcpClient::sockecDisconnected); // TODO: spelling mistake

        disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function

        connect(_socket, &QTcpSocket::disconnected,
                _socket, &QTcpSocket::deleteLater);

        _socket->abort();
    }

    _socket = new QTcpSocket();

    connect(_socket, &QTcpSocket::readyRead,
            this,    &CommTcpClient::dataReady);

    connect(_socket, &QTcpSocket::disconnected,
            this,    &CommTcpClient::sockecDisconnected);

    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function


    // abort all possibly earlier connections
    //_socket->abort();
    _blockSize = 0;
    _socket->connectToHost(QHostAddress::LocalHost, _port);
}

bool CommTcpClient::isConnected()
{
    return _socket->isOpen();
}

void CommTcpClient::dataReady()
{
    // TODO: we could put this to own class
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

    QByteArray messageData;
    in >> messageData;

    emit received(messageData);
}

void CommTcpClient::connectionError(QAbstractSocket::SocketError socketError)
{

    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        // TODO: action

        break;
    case QAbstractSocket::ConnectionRefusedError:
        // TODO: action

        break;
    default:
        // TODO: action .arg(tcpSocket->errorString()));
        break;
    }
}
