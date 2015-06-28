#ifndef QTSIGNALPROXY_H
#define QTSIGNALPROXY_H

#include <QObject>

/**
 * QtSignalProxy can be used to proxy signals.
 */
class QtSignalProxy : public QObject
{
    Q_OBJECT
public:
    explicit QtSignalProxy(QObject *parent = 0);
    virtual ~QtSignalProxy();

    // returns how many time signal has been proxied
    int proxiedCount() const;

signals:
    void proxiedInt(int i);
    void proxiedNoParameters();

public slots:
    // TODO: should be more types here

    virtual void proxyInt(int i);
    virtual void proxyNoParameters();

private:
    int _proxiedCount{0};
};

#endif // QTSIGNALPROXY_H
