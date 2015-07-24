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
 
 #ifndef TARGETDISPLAY_H
#define TARGETDISPLAY_H

#include "display.h"

class TargetDisplay : public Display
{
    Q_OBJECT

public:
    explicit TargetDisplay(int widthPixels, int heightPixels, double xdpi, double ydpi, QObject *parent = 0);
    ~TargetDisplay();

    virtual int widthPixels() const { return _widthPixels; }
    virtual int heightPixels() const { return _heightPixels; }
    virtual double widthInches() const { return _widthPixels / _xdpi; }
    virtual double heightInches() const { return _heightPixels / _ydpi; }
    virtual double xdpi() const { return _xdpi; }
    virtual double ydpi() const { return _ydpi; }

signals:

public slots:

protected:
    TargetDisplay(QObject* parent = 0);

private:
    Q_DISABLE_COPY(TargetDisplay)

    int _widthPixels;
    int _heightPixels;
    double _xdpi;
    double _ydpi;
};

#endif // TARGETDISPLAY_H
