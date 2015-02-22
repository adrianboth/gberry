#include "commtcpclient.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

#include "messagereader.h"


CommTcpClient::CommTcpClient(int port, QObject *parent) :
    QObject(parent), _port(port), _socket(NULL), _reader(NULL)
{

}

CommTcpClient::~CommTcpClient()
{
    if (_socket)
    {
        delete _reader;
        _reader = NULL;

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
        // TODO: dry
        delete _reader;
        _reader = NULL;

        disconnect(_socket, &QTcpSocket::disconnected,
                this,    &CommTcpClient::sockecDisconnected); // TODO: spelling mistake

        disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function

        connect(_socket, &QTcpSocket::disconnected,
                _socket, &QTcpSocket::deleteLater);

        _socket->abort();
    }

    _socket = new QTcpSocket();
    _reader = new MessageReader(_socket);
    // forward signal
    connect(_reader, SIGNAL(received(const QByteArray&)),
            this,    SIGNAL(received(const QByteArray&)));

    connect(_socket, &QTcpSocket::disconnected,
            this,    &CommTcpClient::sockecDisconnected);

    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function


    // abort all possibly earlier connections
    //_socket->abort();
    _socket->connectToHost(QHostAddress::LocalHost, _port);
}

bool CommTcpClient::isConnected()
{
    return _socket->isOpen();
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
