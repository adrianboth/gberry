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
 
 #ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>

class Display : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int widthPixels READ widthPixels)
    Q_PROPERTY(int heightPixels READ heightPixels)
    Q_PROPERTY(double widthInches READ widthInches)
    Q_PROPERTY(double heightInches READ heightInches)
    Q_PROPERTY(double xdpi READ xdpi)
    Q_PROPERTY(double ydpi READ ydpi)

public:
    explicit Display(QObject *parent = 0) : QObject(parent) {}
    ~Display() {}

    virtual int widthPixels() const = 0;
    virtual int heightPixels() const = 0;
    virtual double widthInches() const = 0;
    virtual double heightInches() const = 0;
    virtual double xdpi() const = 0;
    virtual double ydpi() const = 0;

};

#endif // DISPLAY_H

