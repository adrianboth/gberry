#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>

#include "restinvocationfactory.h"

class ServerConnection : public QObject
{
    Q_OBJECT
public:
    explicit ServerConnection(QObject *parent = 0);
    virtual ~ServerConnection();

    virtual bool isConnected() = 0;

    virtual void open() = 0;
    virtual void close() = 0;

signals:
    // we might not need these but usefull in the beginning
    void pingOK();
    void pingFailure();

    void connected();
    void disconnected();

public slots:

};

#endif // SERVERCONNECTION_H
