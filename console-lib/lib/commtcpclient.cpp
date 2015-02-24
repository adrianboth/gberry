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
        close();
}

void CommTcpClient::socketDisconnected()
{
    // someone closed to socket -> reconnect
    open();
}

void CommTcpClient::open()
{
    if (isConnected())
        close();

    _socket = new QTcpSocket();
    _reader = new MessageReader(_socket);
    // forward signal
    connect(_reader, SIGNAL(received(int, const QByteArray&)),
            this,    SIGNAL(received(int, const QByteArray&)));

    connect(_socket, &QTcpSocket::disconnected,
            this,    &CommTcpClient::socketDisconnected);

    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function

    _socket->connectToHost(QHostAddress::LocalHost, _port);
}

void CommTcpClient::close()
{
    delete _reader;
    _reader = NULL;

    disconnect(_socket, &QTcpSocket::disconnected,
               this,    &CommTcpClient::socketDisconnected);

    disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
               this,    SLOT(connectionError(QAbstractSocket::SocketError))); // error(...) overload, any c++11 way to select specific function?

    connect(_socket, &QTcpSocket::disconnected,
            _socket, &QTcpSocket::deleteLater);

    _socket->close();
    _socket = NULL;
}

bool CommTcpClient::isConnected()
{
    return _socket && _socket->state() == QAbstractSocket::ConnectedState;
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

void CommTcpClient::write(int channelId, const QByteArray &msg)
{
    if (_reader)
        _reader->write(channelId, msg);
}
