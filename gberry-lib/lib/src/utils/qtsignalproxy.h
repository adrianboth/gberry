/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #ifndef QTSIGNALPROXY_H
#define QTSIGNALPROXY_H

#include <QObject>

/**
 * QtSignalProxy can be used to proxy signals.
 *
 * One purpose is to avoid declaring *Private classes as QObject child (as they
 * live fully in *.cpp file). In most cases you could already use lambda
 * functions to avoid QtObject but same APIs use old style SLOT() signature.
 * Other reason is to behave correctly if target has possibility to be
 * destroyed before source, lambdas are not automatically tear down.
 *
 * Example usage:
 *
 *   // In *Private setup
 *   QObject::connect(&relaunchProxy, &QtSignalProxy::proxiedNoParameters,
 *                    [this] () { this->relaunchAfterDelay(); });
 *
 *   // In some where else usafe
 *   SystemServices::instance()->singleshotTimer(timeout, &(_d->relaunchProxy), SLOT(proxyNoParameters()));
 *
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
