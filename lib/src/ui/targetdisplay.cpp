#include "targetdisplay.h"

#include <QDebug>

TargetDisplay::TargetDisplay(int widthPixels, int heightPixels, double xdpi, double ydpi, QObject *parent) :
    Display(parent),
    _widthPixels(widthPixels),
    _heightPixels(heightPixels),
    _xdpi(xdpi),
    _ydpi(ydpi)
{
    //qDebug() << "[TargetDisplay]" << width << "x" << height << ", " << xdpi << ":" << ydpi;
}

TargetDisplay::TargetDisplay(QObject *parent) :
    Display(parent),
    _widthPixels(0),
    _heightPixels(0),
    _xdpi(1), // avoid divide by zero
    _ydpi(1)
{

}

TargetDisplay::~TargetDisplay()
{

}
