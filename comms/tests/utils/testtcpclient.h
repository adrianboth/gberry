#ifndef TESTTCPCLIENT_H
#define TESTTCPCLIENT_H

#include <QObject>

class TestTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TestTcpClient(QObject *parent = 0);
    ~TestTcpClient();

signals:

public slots:
};

#endif // TESTTCPCLIENT_H
