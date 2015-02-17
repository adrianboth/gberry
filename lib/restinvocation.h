#ifndef RESTINVOCATION_H
#define RESTINVOCATION_H

#include <QObject>

class RestInvocation : public QObject
{
    Q_OBJECT
public:
    explicit RestInvocation(QObject *parent = 0);
    ~RestInvocation();

signals:

public slots:
};

#endif // RESTINVOCATION_H
