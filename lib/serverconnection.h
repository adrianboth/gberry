#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>

#include "restinvocationfactory.h"


class ServerConnection : public QObject
{
    Q_OBJECT
public:
    explicit ServerConnection(RESTInvocationFactory invocationFactory, QObject *parent = 0);
    ~ServerConnection();

    virtual bool isConnected();

signals:
    void connected();
    void disconnect();

public slots:
};

#endif // SERVERCONNECTION_H
